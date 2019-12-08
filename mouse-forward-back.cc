#include <nan.h>
#include <X11/Xlib.h>

using namespace v8;
using Nan::AsyncResource;

namespace MouseForwardBack {
    uv_loop_t *loop;
    uv_async_t async;
    uv_work_t req;
    Window window;
    Nan::Callback(cb);

    int X_ErrorHandler(Display *d, XErrorEvent *e) {
        char msg[80];
        XGetErrorText(d, e->error_code, msg, sizeof(msg));

        fprintf(stderr, "Error %d (%s): request %d.%d\n", e->error_code, msg, e->request_code, e->minor_code);
        return 0;
    }

    void hook() {
        Display *display;
        XEvent xevent;

        XSetErrorHandler(X_ErrorHandler);

        if( (display = XOpenDisplay(NULL)) == NULL )
            return;

        XAllowEvents(display, AsyncBoth, CurrentTime);

        XGrabButton(display, 8, AnyModifier, window, True,  ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);
        XGrabButton(display, 9, AnyModifier, window, True,  ButtonPressMask | ButtonReleaseMask, GrabModeAsync, GrabModeAsync, None, None);

        // Loop for events.
        while (1) {
        XNextEvent(display, &xevent);
        switch (xevent.type) {
            case ButtonRelease:
                // Sends data back to node
                async.data = (void*) &xevent.xbutton.button;
                uv_async_send(&async);
                break;
        }
        }
    }

    void events_callback(uv_async_t *handle) {
        Nan::HandleScope scope;
        uint buttonNumber = *((uint*) handle->data);

        Local<Value> arg = Nan::New(buttonNumber);

        cb.Call(1, &arg);
    }

    void OnFatalError(const char* errorMessage, const char* more) {
        printf("%s :: %s", errorMessage, more);
    }

    NAN_METHOD(GetEvents) {
        Isolate* isolate = info.GetIsolate();
        isolate->SetFatalErrorHandler(OnFatalError);
        
        // Callback
        Local<Function> cb = info[0].As<Function>();

        // Window handle
        Local<Value> value = info[1];
        if (node::Buffer::HasInstance(value)) {
            Window* data = reinterpret_cast<Window*>(node::Buffer::Data(value));
            window = *data;
        }

        // Start thread
        loop = uv_default_loop();

        int param = 0;
        uv_thread_t t_id;
        uv_thread_cb uvcb = (uv_thread_cb) hook;

        uv_async_init(loop, &async, events_callback);
        uv_thread_create(&t_id, uvcb, &param);
    }

    NAN_MODULE_INIT(Init)
    {
        Nan::SetMethod(target, "register", GetEvents);
    }

    NODE_MODULE(mouse, Init)

}