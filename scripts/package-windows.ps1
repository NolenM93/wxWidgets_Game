# Packages CodeBlooded.exe plus wxWidgets DLLs and the MSVC runtime
# into dist/CodeBlooded-windows-x64.zip for a clean Windows 10/11 machine.
$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$stage = Join-Path $root "dist\stage"
$zip = Join-Path $root "dist\CodeBlooded-windows-x64.zip"

function Find-Exe {
    $candidates = @(
        (Join-Path $root "build\bin\Release\CodeBlooded.exe"),
        (Join-Path $root "build\Release\CodeBlooded.exe"),
        (Join-Path $root "build\bin\CodeBlooded.exe")
    )
    foreach ($path in $candidates) {
        if (Test-Path $path) { return Get-Item $path }
    }
    $found = Get-ChildItem -Path (Join-Path $root "build") -Recurse -Filter CodeBlooded.exe -ErrorAction SilentlyContinue |
        Where-Object { $_.FullName -match "Release" } |
        Select-Object -First 1
    if ($found) { return $found }
    throw "CodeBlooded.exe not found under build/. Build the Release target first."
}

function Find-MsvcCrt {
    # Get-Item returns the CRT directories themselves. Get-ChildItem on the same
    # glob lists their DLL contents, and -Directory then matches nothing.
    $matches = @()
    $vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vswhere) {
        $installs = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
        foreach ($install in @($installs)) {
            if ($install) {
                $matches += @(Get-Item (Join-Path $install "VC\Redist\MSVC\*\x64\Microsoft.VC*.CRT") -ErrorAction SilentlyContinue)
            }
        }
    }
    $matches += @(Get-Item "C:\Program Files\Microsoft Visual Studio\*\*\VC\Redist\MSVC\*\x64\Microsoft.VC*.CRT" -ErrorAction SilentlyContinue)
    return $matches | Where-Object { $_ -and $_.PSIsContainer } | Select-Object -Last 1
}

$exe = Find-Exe
Write-Host "Packaging $($exe.FullName)"

Remove-Item (Join-Path $root "dist") -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force $stage | Out-Null
Copy-Item $exe.FullName $stage

$exeDir = $exe.DirectoryName
Get-ChildItem $exeDir -Filter *.dll -ErrorAction SilentlyContinue | Copy-Item -Destination $stage

$vcpkgBins = @(
    (Join-Path $root "build\vcpkg_installed\x64-windows\bin"),
    (Join-Path $root "vcpkg_installed\x64-windows\bin")
)
foreach ($bin in $vcpkgBins) {
    if (Test-Path $bin) {
        Get-ChildItem $bin -Filter *.dll | Copy-Item -Destination $stage -Force
    }
}

$vcpkgRoot = $env:VCPKG_ROOT
if (-not $vcpkgRoot) { $vcpkgRoot = $env:VCPKG_INSTALLATION_ROOT }
$applocal = $null
if ($vcpkgRoot) {
    $applocal = Join-Path $vcpkgRoot "scripts\buildsystems\msbuild\applocal.ps1"
}
if ($applocal -and (Test-Path $applocal)) {
    $installed = $vcpkgBins | Where-Object { Test-Path $_ } | Select-Object -First 1
    if ($installed) {
        Write-Host "Running vcpkg applocal from $installed"
        & $applocal -targetBinary (Join-Path $stage "CodeBlooded.exe") -installedDir (Split-Path $installed -Parent)
    }
}

$redist = Find-MsvcCrt
if (-not $redist) {
    throw "MSVC redistributable CRT not found. Refusing to pack a zip that will not run on a clean PC."
}
Write-Host "Copying MSVC runtime from $($redist.FullName)"
Copy-Item (Join-Path $redist.FullName "*.dll") $stage -Force

$crtDll = Join-Path $stage "vcruntime140.dll"
if (-not (Test-Path $crtDll)) {
    throw "vcruntime140.dll was not copied into the package."
}

$examples = Join-Path $root "examples"
if (Test-Path $examples) {
    Copy-Item $examples (Join-Path $stage "examples") -Recurse
}

@"
Code Blooded
============

Double-click CodeBlooded.exe. No installer is required.

1. The left panel is a bot script. A duel script is loaded by default.
2. Click Run Simulation. Both robots execute the script every tick.
3. Click Stop to pause.

Commands: moveTo(x, y), moveToNearest(), activateShield(),
takeDamage(amount), attackNearest(range, damage). Lines starting
with # or // are comments.

If Windows asks for the VC++ runtime, install the x64 Microsoft
Visual C++ Redistributable, then run the exe again.
"@ | Set-Content -Path (Join-Path $stage "README.txt") -Encoding UTF8

if (Test-Path $zip) { Remove-Item $zip -Force }
Compress-Archive -Path (Join-Path $stage "*") -DestinationPath $zip
Write-Host "Wrote $zip"
Get-ChildItem $stage | Format-Table Name, Length
