{
  'variables': {
    'tclconfig%': '<!(tclsh gyp/tclconfig.tcl)'
  },
  'targets': [
    {
      'target_name': 'tcl',
      'sources': [
        'src/tclbinding.cpp'
      ],
      'variables': {
        'tclthreads': '<!(. <(tclconfig) && echo ${TCL_THREADS})'
      },
      'conditions': [
        [ 'OS=="mac"', {
          'xcode_settings': {
            'OTHER_CPLUSPLUSFLAGS' : ['-std=c++11','-stdlib=libc++'],
            'OTHER_LDFLAGS': ['-stdlib=libc++'],
            'MACOSX_DEPLOYMENT_TARGET': '10.7'
          }
        } ],
        [ 'tclthreads==1', {
          'defines': [
            'ENABLE_THREADS'
          ],
          'sources': [
            'src/tclworker.cpp',
            'src/taskrunner.cpp',
            'src/asynchandler.cpp'
          ]
        } ]
      ],
      'include_dirs': [
        '<!(. <(tclconfig) && echo ${TCL_INCLUDE_SPEC} | sed s/-I//g)',
        '<!(node -e "require(\'nan\')")'
      ],
      'cflags': [
        '-std=c++11'
      ],
      'link_settings': {
        'libraries': [
          '<!(. <(tclconfig) && echo ${TCL_LIB_SPEC})'
        ]
      }
    }
  ]
}
