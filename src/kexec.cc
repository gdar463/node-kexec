#include <cstdio>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <string>
#include <unistd.h>

#include <napi.h>
#include <uv.h>

using namespace Napi;

static int clear_cloexec(int desc) {
  int flags = fcntl(desc, F_GETFD, 0);
  if (flags < 0)
    return flags; // return if reading failed

  flags &= ~FD_CLOEXEC; // clear FD_CLOEXEC bit
  return fcntl(desc, F_SETFD, flags);
}

static int do_exec(char *argv[]) {
  clear_cloexec(0); // stdin
  clear_cloexec(1); // stdout
  clear_cloexec(2); // stderr
  return execvp(argv[0], argv);
}

Value kexec(const CallbackInfo &info) {
  Env env = info.Env();
  /*
   * Steve Blott: 17 Jan, 2014
   *              Temporary comment by way of explanation...
   *              To be deleted.
   *
   * With a single argument:
   *   - pass it to execvp as "sh -c 'args[0]'"
   *   - this is the existing usage
   *
   * With exactly two arguments:
   *   - the first is the command name
   *   - the second is an array of arguments
   *     ...as in process.child_process.spawn()
   *
   * This approach is not great, but it allows the established usage to
   * coexist with direct execvp-usage, and avoids making any changes to the
   * established API.
   */

  if (1 == info.Length() && info[0].IsString()) {
    String str(env, info[0]);
    std::string s = str.Utf8Value();
    char *argv[] = {const_cast<char *>("/bin/sh"), const_cast<char *>("-c"),
                    s.data(), NULL};

    int err = do_exec(argv);

    return Number::New(env, err);
  }

  if (2 == info.Length() && info[0].IsString() && info[1].IsArray()) {
    String str(env, info[0]);

    // Substantially copied from:
    //
    // https://github.com/joyent/node/blob/2944e03/src/node_child_process.cc#L92-104
    Array argv_handle = info[1].As<Array>();
    int argc = argv_handle.Length();

    int argv_length = argc + 1 + 1;
    char **argv = new char *[argv_length];

    std::string s = str.Utf8Value();
    argv[0] = s.data();
    argv[argv_length - 1] = NULL;
    for (int i = 0; i < argc; i++) {
      String arg(env, argv_handle.Get(Number::New(env, i)).ToString());
      argv[i + 1] = strdup(arg.Utf8Value().c_str());
    }

    int err = do_exec(argv);

    for (int i = 0; i < argc; i++)
      free(argv[i + 1]);
    delete[] argv;

    return Number::New(env, err);
  }

  TypeError::New(env, "kexec: invalid arguments").ThrowAsJavaScriptException();
  return env.Null();
}

Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "kexec"), Function::New<kexec>(env));
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init);
