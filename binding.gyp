{
  "targets": [
    {
      "target_name": "tcl",
      "sources": [
        "src/init.cpp"
      ],
      "include_dirs": [
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
