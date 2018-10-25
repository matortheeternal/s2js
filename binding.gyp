{
  "targets": [
    {
      "target_name": "s2js",
      "sources": [
        "src/cpp/api.cc",
        "src/cpp/s2js.cc",
        "src/cpp/unit_classes.cc"
      ],
      "libraries": [
        "$(S2API_ROOT)/$(BUILD_FOLDER)/bin/civetweb",
        "$(S2API_ROOT)/$(BUILD_FOLDER)/bin/libprotobufd",
        "$(S2API_ROOT)/$(BUILD_FOLDER)/bin/sc2apid",
        "$(S2API_ROOT)/$(BUILD_FOLDER)/bin/sc2libd",
        "$(S2API_ROOT)/$(BUILD_FOLDER)/bin/sc2protocold",
        "$(S2API_ROOT)/$(BUILD_FOLDER)/bin/sc2utilsd"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "$(S2API_ROOT)/include",
        "$(S2API_ROOT)/$(BUILD_FOLDER)/generated",
        "$(S2API_ROOT)/contrib/protobuf/src",
        "$(S2API_ROOT)/contrib/civetweb/include"
      ]
    }
  ],
}