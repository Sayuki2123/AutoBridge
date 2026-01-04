# Configurable variables
$abis = @("x86_64", "x86")
$ANDROID_NDK_HOME = $env:ANDROID_NDK_HOME
$ANDROID_PLATFORM = 30
$BUILD_TYPE = "Release"
$GENERATOR = "Ninja"

# Build commands
$abis | ForEach-Object {
    $abi = $_
    $buildDir = "build/$abi"

    if (-not (Test-Path $buildDir)) {
        mkdir "$buildDir" | Out-Null

        Start-Process -FilePath "cmake" -NoNewWindow -Wait -ArgumentList @(
            "-B $buildDir"
            "-G $GENERATOR"
            "-DANDROID_ABI=$abi"
            "-DANDROID_NDK=$ANDROID_NDK_HOME"
            "-DANDROID_PLATFORM=$ANDROID_PLATFORM"
            "-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
            "-DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake"
        )
    }

    $exitCode = (Start-Process -FilePath "cmake" -ArgumentList "--build $buildDir" -NoNewWindow -Wait -PassThru).ExitCode

    if ($exitCode -eq 1) {
        Write-Warning "Build failed"
        pause
        exit
    }

    $targetDir = "template/system/lib"

    if ($abi.Contains("64")) {
        $targetDir += "64"
    }

    Copy-Item -Path "$buildDir/lib/*" -Destination $targetDir -Force
}

Add-Type -AssemblyName System.IO.Compression.FileSystem

class FixedUTF8 : System.Text.UTF8Encoding {
    FixedUTF8() : base($true) { }

    [byte[]] GetBytes([string] $s) {
        $s = $s.Replace("\", "/");
        return ([System.Text.UTF8Encoding]$this).GetBytes($s);
    }
}

$zipFile = "build/AutoBridge.zip"

if (Test-Path $zipFile) {
    Remove-Item $zipFile
}

Write-Output "Packing module files..."

try {
    [System.IO.Compression.ZipFile]::CreateFromDirectory("template", $zipFile, "Optimal", $false, [FixedUTF8]::new())
    Write-Output "Module files have been packaged into $zipFile"
}
catch {
    Write-Warning $_.Exception.Message
}

pause
