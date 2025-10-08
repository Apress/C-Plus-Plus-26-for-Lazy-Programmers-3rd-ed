$template = "template"

Get-ChildItem -Directory | ForEach-Object {
    $folder = $_.Name
    $target = $_.FullName
    if (Test-Path "$target\$folder.vcxproj") {
        Copy-Item "$template\basic.vcxproj" -Destination "$target\$folder.vcxproj" -Force
        Copy-Item "$template\basic.vcxproj.user" -Destination "$target\$folder.vcxproj.user" -Force
        Copy-Item "$template\basic.vcxproj.filters" -Destination "$target\$folder.vcxproj.filters" -Force
    }
}
