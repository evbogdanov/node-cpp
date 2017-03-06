#include <node.h>
#include <uv.h>
#include <unistd.h>

#define ERR(str) {\
                  auto err = String::NewFromUtf8(isolate, str);\
                  isolate->ThrowException(Exception::TypeError(err));\
                  return;\
                 }

namespace node_cpp {
////////////////////////////////////////////////////////////////////////////////

using v8::FunctionCallbackInfo;
using v8::Value;
using v8::Isolate;
using v8::String;
using v8::Exception;
using v8::Number;
using v8::Local;
using v8::Object;
using v8::Persistent;
using v8::Function;
using v8::HandleScope;

// SYNC
// -----------------------------------------------------------------------------

void
Sum(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();

    if (args.Length() != 2) ERR("Wrong number of arguments");
            
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) ERR("Wrong arguments");

    double arg1 = args[0]->NumberValue();
    double arg2 = args[1]->NumberValue();
    double sum = arg1 + arg2;
    auto sum_num = Number::New(isolate, sum);

    args.GetReturnValue().Set(sum_num);
}

void
Twice(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();

    if (args.Length() != 1 || !args[0]->IsNumber()) ERR("Where is my numeric argument?");

    double arg = args[0]->NumberValue();
    double twice = arg * 2;
    auto twice_num = Number::New(isolate, twice);

    args.GetReturnValue().Set(twice_num);
}

// ASYNC
// -----------------------------------------------------------------------------

struct Work {
    uv_work_t request;
    Persistent<Function> callback;
    double input;
    double output;
};

static void
WorkBegin(uv_work_t* req)
{
    Work* work = static_cast<Work*>(req->data);
    sleep(1); // Fake heavy calculation
    work->output = work->input * 2;
}

static void
WorkEnd(uv_work_t* req, int status)
{
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope handleScope(isolate); // Required for Node 7.x

    Work* work = static_cast<Work*>(req->data);
    
    // Set up returning arguments and execute the callback
    auto ctx = isolate->GetCurrentContext()->Global();
    const int argc = 1;
    Local<Value> argv[argc] = { Number::New(isolate, work->output) };
    Local<Function>::New(isolate, work->callback)->Call(ctx, argc, argv);
    
    // Clean up
    work->callback.Reset();
    delete work;
}

void
TwiceAsync(const FunctionCallbackInfo<Value>& args)
{
    Isolate* isolate = args.GetIsolate();

    if (args.Length() != 2) ERR("Wrong number of arguments");
            
    if (!args[0]->IsNumber() || !args[1]->IsFunction()) ERR("Wrong arguments");

    Work* work = new Work();
    work->request.data = work;

    // Store JS input
    double input = args[0]->NumberValue();
    work->input = input;

    // Store JS callback so I can invoke it later
    Local<Function> callback = Local<Function>::Cast(args[1]);
    work->callback.Reset(isolate, callback);

    // Start worker thread
    uv_queue_work(uv_default_loop(), &work->request, WorkBegin, WorkEnd);

    // Return nothing (result will be calculated in the worker thread)
    args.GetReturnValue().Set(Undefined(isolate));
}

// EXPORTS
// -----------------------------------------------------------------------------

void
Init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "sum", Sum);
    NODE_SET_METHOD(exports, "twice", Twice);
    NODE_SET_METHOD(exports, "twiceAsync", TwiceAsync);
}

NODE_MODULE(addon, Init)

////////////////////////////////////////////////////////////////////////////////
}
