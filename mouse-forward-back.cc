#include <nan.h>
#include <node.h>
#include <X11/Xlib.h>
#include <iostream>

using namespace v8;

static Persistent<Function> persistentCallback;
uv_loop_t *loop;
uv_async_t async;
uv_work_t req;
Window window;


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
    uint buttonNumber = *((uint*) handle->data);

    const unsigned argc = 1;

    Isolate* isolate = Isolate::GetCurrent();
    HandleScope scope(isolate);

    Local<Value> argv[argc] = { Number::New(isolate, buttonNumber) };

    Local<Function> f = Local<Function>::New(isolate, persistentCallback);
    f->Call(isolate->GetCurrentContext()->Global(), argc, argv);

}

void OnFatalError(const char* message, const char* somethingElse) {
    std::cout << message << " ::::: " << somethingElse << std::endl;
}

void RunCallback(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = Isolate::GetCurrent();

    HandleScope scope(isolate);

    // Keep the application running even if something fails.
    isolate->SetFatalErrorHandler(OnFatalError);

    Handle<Function> cb = Handle<Function>::Cast(args[0]);
    persistentCallback.Reset(isolate, cb);

    Local<Value> value = args[1];
    if (node::Buffer::HasInstance(value)) {
      Window* data = reinterpret_cast<Window*>(node::Buffer::Data(value));
      window = *data;
    }

    loop = uv_default_loop();

    int param = 0;
    uv_thread_t t_id;
    uv_thread_cb uvcb = (uv_thread_cb) hook;

    uv_async_init(loop, &async, events_callback);
    uv_thread_create(&t_id, uvcb, &param);
}

void init(Local<Object> exports, Local<Object> module)
{
    NODE_SET_METHOD(module, "exports", RunCallback);
}

NODE_MODULE(mouse, init)
