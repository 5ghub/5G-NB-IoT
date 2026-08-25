# Compiles every sketch under the repo's tracked scope against the 5G-NB-IoT board
# and writes a CSV + text log report. Does not touch TESTING_CHECKLIST.md.

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$cli = "$env:LOCALAPPDATA\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe"

if (-not (Test-Path $cli)) {
    throw "arduino-cli.exe not found at: $cli`nIs Arduino IDE 2.3.10 installed at the default location?"
}

$fqbn = "5G-NB-IoT:samd:5G-NB-IoT"

# Same scope as TESTING_CHECKLIST.md
$scopeDirs = @("ArduinoSketches", "KitSketches", "5G OBD", "NBIoTPhone", "u-blox_GNSS")

$sketchDirs = foreach ($d in $scopeDirs) {
    $full = Join-Path $repoRoot $d
    if (Test-Path $full) {
        Get-ChildItem -Path $full -Recurse -Filter *.ino |
            Select-Object -ExpandProperty DirectoryName -Unique
    }
}
$sketchDirs = $sketchDirs | Sort-Object -Unique

Write-Host "Found $($sketchDirs.Count) sketches to compile against $fqbn`n"

$results = @()
$i = 0
foreach ($dir in $sketchDirs) {
    $i++
    $name = Split-Path $dir -Leaf
    $relPath = $dir.Substring($repoRoot.Length + 1) -replace '\\', '/'
    Write-Host "[$i/$($sketchDirs.Count)] Compiling $name ..." -NoNewline

    $pass = $false
    $output = ""
    try {
        $cmdLine = "`"$cli`" compile --fqbn $fqbn `"$dir`" 2>&1"
        $outLines = & cmd /c $cmdLine
        $output = ($outLines -join "`n")
        $pass = ($LASTEXITCODE -eq 0)
    } catch {
        $output = "Script-level error while compiling: $($_.Exception.Message)"
        $pass = $false
    }

    if ($pass) {
        Write-Host " OK" -ForegroundColor Green
    } else {
        Write-Host " FAIL" -ForegroundColor Red
    }

    $lastLine = ($output.Trim() -split "`r?`n" | Select-Object -Last 1)

    $results += [PSCustomObject]@{
        Sketch  = $name
        Path    = $relPath
        Pass    = $pass
        Summary = $lastLine
        Output  = $output.Trim()
    }
}

$reportPath = Join-Path $repoRoot "compile_report.csv"
$results | Select-Object Sketch, Path, Pass, Summary |
    Export-Csv -Path $reportPath -NoTypeInformation -Encoding UTF8

$logPath = Join-Path $repoRoot "compile_report.log"
$logLines = foreach ($r in $results) {
    $status = if ($r.Pass) { "PASS" } else { "FAIL" }
    "=== $($r.Path) - $status ===`n$($r.Output)`n"
}
$logLines | Set-Content -Path $logPath -Encoding UTF8

$passCount = ($results | Where-Object { $_.Pass }).Count
Write-Host "`n$passCount / $($results.Count) compiled successfully."
Write-Host "CSV summary: $reportPath"
Write-Host "Full output log: $logPath"
