{
  "targets": [
    {
      "target_name": "tcl",
      "sources": [
        "src/binding.cpp",
        "src/tclinterp.cpp"
      ],
      "include_dirs": [
        "include",
        "<!(node -e \"require('nan')\")"
      ],
      "link_settings": {
        "libraries": [
          "-ltcl"
        ]
      }
    }
  ]
}
