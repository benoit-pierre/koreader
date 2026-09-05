def asset_parse:
  . as $file
  | {
    # Android:
    "android-arm": "Android ARM",
    "android-arm64": "Android ARM64",
    # Kindle:
    "kindle": "Kindle",
    "kindle-legacy": "Kindle Legacy",
    "kindlehf": "Kindle HF",
    "kindlepw2": "Kindle PW2",
    # Linux:
    "linux-aarch64": "Linux ARM64",
    "linux-amd64": "Linux x86_64",
    "linux-arm64": "Linux ARM64",
    "linux-armhf": "Linux ARMhf",
    "linux-x86_64": "Linux x86_64",
  } as $platform_name
  | "(?<platform>.+)" as $platform_rx
  | "v(?<version>(?<base_version>[0-9]+(\\.[0-9]+)*)(-(?<commit_number>[0-9]+)-g(?<commit_hash>[a-f0-9]+))?)" as $version_rx
  | "\\.(?<extension>[^.]+(\\.[^.]+)*)$" as $extension_rx
  | $file | (
    # koreader-linux-x86_64-v2023.06.1.tar.xz
    # koreader-ubuntu-touch-arm-v2015.11-640-g17e9a8e_2018-03-09.targz
    # koreader-android-arm-v2015.11-654-gb7392f7_2018-03-09.apk
    capture("/?koreader-" + $platform_rx + "-" + $version_rx + $extension_rx)
    # koreader-v2023.06.1-x86_64.AppImage
    # koreader-v2025.10-197-g7c5ee9c1a2_2026-03-13-x86_64.AppImage
    // (
         capture("/?koreader-" + $version_rx + "-" + $platform_rx + $extension_rx)
         | .platform = "linux-" + .platform
    )
    # koreader_v2026.09-8-g84cf973-1_amd64.deb
    // (
         capture("/?koreader_" + $version_rx + "-1_" + $platform_rx + $extension_rx)
         | .platform = "linux-" + .platform
    )
    # koreader-kindlepw2-latest-nightly.kotasync
    # koreader-kindlepw2-latest-stable.zsync
    // (
      capture("/?koreader-" + $platform_rx + "-latest-(?<version>nightly|stable)" + $extension_rx)
      | .base_version = .version
      | .sort_version = [666]
      | .latest_ota = true
    )
    // error("unsupported asset: " + .)
  ) #| debug
  # Finalize.
  | .file = $file
  | .platform_name = ($platform_name[.platform] // error("invalid platform: " + .platform))
  | .sort_version = (.sort_version // [
    (.base_version | split(".") | map(tonumber)),
    (.commit_number // 0 | tonumber)
  ])
  | .sort_key = [.sort_version, .platform, .extension]
 #| debug
;

# vim: sw=2
