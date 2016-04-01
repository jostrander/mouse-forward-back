Mouse Back / Forward for Electron Linux
======================================================

A module used to listen for buttons 8/9 for a window in Electron.

## Usage

Requiring the module returns an object with one method:

### register(fn, windowHandle)
This will bind the callback `fn` to any mouse forward/back events for `windowHandle`. `fn` takes one parameter, and will return either `back` or `forward`.

In electron you need to pass in `yourWindow.getNativeWindowHandle()` as the second parameter.

If you are not using Electron, `windowHandle` is expecting a `node::Buffer` of a `Window` for X11.

#### License

The MIT License (MIT)
Copyright (c) 2016 Jesse Ostrander

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
