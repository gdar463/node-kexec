{
  'targets': [
    {
      'target_name': 'kexec',
      'cflags!': [ '-fno-exceptions' ],
      'cflags_cc!': [ '-fno-exceptions' ],
      'xcode_settings': { 'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7',
      },
      'msvs_settings': {
        'VCCLCompilerTool': { 'ExceptionHandling': 1 },
      },
      'sources': [ 'src/kexec.cc' ],
      'defines': [
        '<!@(node -v |grep "v[^0]" > /dev/null && echo "__NODE_GE_V4__" || true)',
        '<!@(node -v |grep "v0\.1[12]" > /dev/null && echo "__NODE_V0_11_OR_12__" || true)',
        '<!@(command -v iojs > /dev/null && echo "__NODE_V0_11_OR_12__" || true)',
        '<!@(node -v |grep "v0\.10" > /dev/null && echo "__NODE_V0_10__" || true)',
      ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include_dir\")"]
    }
  ]
}
