{
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
            },
			"variables": {
			         "TCL_LIB" : "-ltcl"
            }
        } ],
        ["OS=='linux'", {
			"variables": {
			         "TCL_LIB" : "-ltcl<!(echo 'puts $tcl_version' | tclsh )",
            }
        } ]
      ],
      "include_dirs": [
        "include",
        "<!(node -e \"require('nan')\")"
      ],
      "cflags": [
        "-std=c++11"
      ],
      "link_settings": {
        "libraries": [
          "<(TCL_LIB)"
        ]
      }
    }
  ]
}
