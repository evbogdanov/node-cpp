#include <node.h>

#define ERR(str) {\
                  auto err = String::NewFromUtf8(isolate, str);\
                  isolate->ThrowException(Exception::TypeError(err));\
                  return;\
                 }

namespace node_cpp {
    using v8::FunctionCallbackInfo;
    using v8::Value;
    using v8::Isolate;
    using v8::String;
    using v8::Exception;
    using v8::Number;
    using v8::Local;
    using v8::Object;

    void Sum(const FunctionCallbackInfo<Value>& args)
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

    void Twice(const FunctionCallbackInfo<Value>& args)
    {
        Isolate* isolate = args.GetIsolate();

        if (args.Length() != 1 || !args[0]->IsNumber()) ERR("Where is my numeric argument?");

        double arg = args[0]->NumberValue();
        double twice = arg * 2;
        auto twice_num = Number::New(isolate, twice);

        args.GetReturnValue().Set(twice_num);
    }

    void Init(Local<Object> exports)
    {
        NODE_SET_METHOD(exports, "sum", Sum);
        NODE_SET_METHOD(exports, "twice", Twice);
    }

    NODE_MODULE(addon, Init)
}
