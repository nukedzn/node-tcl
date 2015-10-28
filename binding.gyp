{
  "variables": {
    "tclconfig%": "<!(tclsh gyp/tclconfig.tcl)"
  },
  "targets": [
    {
      "target_name": "tcl",
      "sources": [
        "src/tclbinding.cpp",
        "src/tclworker.cpp",
        "src/taskrunner.cpp",
        "src/asynchandler.cpp"
      ],
      "conditions": [
        [ 'OS=="mac"', {
            "xcode_settings": {
              "OTHER_CPLUSPLUSFLAGS" : ["-std=c++11","-stdlib=libc++"],
              "OTHER_LDFLAGS": ["-stdlib=libc++"],
              "MACOSX_DEPLOYMENT_TARGET": "10.7"
            }
        } ]
      ],
      "include_dirs": [
        "<!(. <(tclconfig) && echo ${TCL_INCLUDE_SPEC} | sed s/-I//g)",
        "<!(node -e \"require('nan')\")"
      ],
      "cflags": [
        "-std=c++11"
      ],
      "link_settings": {
        "libraries": [
          "<!(. <(tclconfig) && echo ${TCL_LIB_SPEC})"
        ]
      }
    }
  ]
}
