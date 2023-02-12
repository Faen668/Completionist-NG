#Requires -Version 5

# args
param (
    [Parameter(Mandatory)][ValidateSet('COPY', 'SOURCEGEN', 'DISTRIBUTE')][string]$Mode,
    [string]$Version,
    [string]$Path,
    [string]$Project
)


$ErrorActionPreference = "Stop"

$Folder = $PSScriptRoot | Split-Path -Leaf
$SourceExt = @('.c', '.cpp', '.cxx', '.h', '.hpp', '.hxx')
$ConfigExt = @('.ini', '.json', '.toml')
$env:ScriptCulture = (Get-Culture).Name -eq 'zh-CN'


function L {
	param (
		[Parameter(Mandatory)][string]$en,
		[string]$zh = ''
	)
	
	process {
		if ($env:ScriptCulture -and $zh) {
			return $zh
		}
		else {
			return $en
		}
	}
}

function Resolve-Files {
    param (
        [Parameter(ValueFromPipeline)][string]$a_parent = $PSScriptRoot,
        [string[]]$a_directory = @('include', 'src', 'test')
    )
    
    process {
        Push-Location $PSScriptRoot
        $_generated = [System.Collections.ArrayList]::new()

        try {
            foreach ($directory in $a_directory) {
                if (!$env:RebuildInvoke) {
                    Write-Host "`t[$a_parent/$directory]"
                }

                Get-ChildItem "$a_parent/$directory" -Recurse -File -ErrorAction SilentlyContinue | Where-Object {
                    ($_.Extension -in $SourceExt) -and 
                    ($_.Name -notmatch 'Plugin.h|Version.h')
                } | Resolve-Path -Relative | ForEach-Object {
                    if (!$env:RebuildInvoke) {
                        Write-Host "`t`t<$_>"
                    }
                    $_generated.Add("`n`t`"$($_.Substring(2) -replace '\\', '/')`"") | Out-Null
                }
            }               
            
            Get-ChildItem "$a_parent" -File -ErrorAction SilentlyContinue | Where-Object {
                ($_.Extension -in $ConfigExt) -and 
                ($_.Name -notmatch 'cmake|vcpkg')
            } | Resolve-Path -Relative | ForEach-Object {
                if (!$env:RebuildInvoke) {
                    Write-Host "`t`t<$_>"
                }
                $_generated.Add("`n`t`"$($_.Substring(2) -replace '\\', '/')`"") | Out-Null
            }
        } finally {
            Pop-Location
        }

        return $_generated
    }
}


Write-Host "`n`t<$Folder> [$Mode]"


# @@COPY
if ($Mode -eq 'COPY') {
    # process newly added files
    $BuildFolder = Get-ChildItem (Get-Item $Path).Parent.Parent.FullName "$Project.sln" -Depth 2 -File -Exclude ('*CMakeFiles*', '*CLib*')
    $NewFiles = Get-ChildItem $BuildFolder.DirectoryName -File | Where-Object {$_.Extension -in $SourceExt}
    if ($NewFiles) { # trigger ZERO_CHECK
        $NewFiles | Move-Item -Destination "$PSScriptRoot/src" -Force -Confirm:$false -ErrorAction:SilentlyContinue | Out-Null
        [IO.File]::WriteAllText("$PSScriptRoot/CMakeLists.txt", [IO.File]::ReadAllText("$PSScriptRoot/CMakeLists.txt"))
    }

    # Build Target
    Write-Host "`t$Folder $Version"
    $vcpkg = [IO.File]::ReadAllText("$PSScriptRoot/vcpkg.json") | ConvertFrom-Json
    $Install = $vcpkg.'features'.'mo2-install'.'description'
    $ProjectCMake = [IO.File]::ReadAllText("$PSScriptRoot/CMakeLists.txt")
    $OldVersion = [regex]::match($ProjectCMake, '(?s)(?:(?<=\sVERSION\s)(.*?)(?=\s+))').Groups[1].Value


	Add-Type -AssemblyName Microsoft.VisualBasic
    Add-Type -AssemblyName System.Windows.Forms
    Add-Type -AssemblyName System.Drawing

    [System.Windows.Forms.Application]::EnableVisualStyles()
    $MsgBox = New-Object System.Windows.Forms.Form -Property @{
        TopLevel = $true
        ClientSize = '350, 305'
        Text = $Project
        StartPosition = 'CenterScreen'
        FormBorderStyle = 'FixedDialog'
        MaximizeBox = $false
        MinimizeBox = $false
        Font = New-Object System.Drawing.Font('Segoe UI', 10, [System.Drawing.FontStyle]::Regular)
    }
    
    $Message = New-Object System.Windows.Forms.ListBox -Property @{
        ClientSize = '225, 150'
        Location = New-Object System.Drawing.Point(20, 20)
    }
    
    function Log {
        param (
            [Parameter(ValueFromPipeline)][string]$a_log
        )

        process {
            $Message.Items.Add($a_log)
            $Message.SelectedIndex = $Message.Items.Count - 1;
            $Message.SelectedIndex = -1;
        }
    }
    
    function Copy-Mod {
        param (
            $Data
        )

        New-Item -Type Directory "$Data/SKSE/Plugins" -Force | Out-Null

        # binary
        Copy-Item "$Path/$Project.dll" "$Data/SKSE/Plugins/$Project.dll" -Force
        "- Binary files copied" | Log

        # configs
        Get-ChildItem $PSScriptRoot | Where-Object {
            ($_.Extension -in $ConfigExt) -and 
            ($_.Name -notmatch 'CMake|vcpkg')
        } | ForEach-Object {
            Copy-Item $_.FullName "$Data/SKSE/Plugins/$($_.Name)" -Force
            "- Configuration files copied" | Log
        }

        # shockwave
        if (Test-Path "$PSScriptRoot/Interface/*.swf" -PathType Leaf) {
            New-Item -Type Directory "$Data/Interface" -Force | Out-Null
            Copy-Item "$PSScriptRoot/Interface" "$Data" -Recurse -Force
            "- Shockwave files copied" | Log
        }

        # papyrus
        if (Test-Path "$PSScriptRoot/Scripts/*.pex" -PathType Leaf) {
            New-Item -Type Directory "$Data/Scripts" -Force | Out-Null
            xcopy.exe "$PSScriptRoot/Scripts" "$Data/Scripts" /C /I /S /E /Y
            "- Papyrus scripts copied" | Log
        }
        if (Test-Path "$PSScriptRoot/Scripts/Source/*.psc" -PathType Leaf) {
            New-Item -Type Directory "$Data/Scripts/Source" -Force | Out-Null
            xcopy.exe "$PSScriptRoot/Scripts/Source" "$Data/Scripts/Source" /C /I /S /E /Y
            "- Papyrus scripts copied" | Log
        }
    }

    $BtnCopyMO2 = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'Copy to MO2'
        Location = New-Object System.Drawing.Point(260, 19)
        BackColor = 'Cyan'
        Add_Click = {
            foreach ($runtime in @("$($env:MO2SkyrimAEPath)/mods", "$($env:MO2SkyrimSEPath)/mods", "$($env:MO2SkyrimVRPath)/mods")) {
                if (Test-Path $runtime -PathType Container) {
                    Copy-Mod "$runtime/$Install"
                }
            }
            "- Copied to MO2." | Log
        }
    }
    
    $BtnCopyData = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'Copy to Data'
        Location = New-Object System.Drawing.Point(260, 74)
        Add_Click = {
            foreach ($runtime in @("$($env:SkyrimAEPath)/data", "$($env:SkyrimSEPath)/data", "$($env:SkyrimVRPath)/data")) {
                if (Test-Path $runtime -PathType Container) {
                    Copy-Mod "$runtime"
                }
            }
            "- Copied to game data." | Log
        }
    }
    
    $BtnRemoveData = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'Remove in Data'
        Location = New-Object System.Drawing.Point(260, 129)
        Add_Click = {
            foreach ($runtime in @("$($env:SkyrimAEPath)/data", "$($env:SkyrimSEPath)/data", "$($env:SkyrimVRPath)/data")) {
                if (Test-Path "$runtime/SKSE/Plugins/$Project.dll" -PathType Leaf) {
                    Remove-Item "$runtime/SKSE/Plugins/$Project.dll" -Force -Confirm:$false -ErrorAction:SilentlyContinue | Out-Null
                }
            }
            "- Removed from game data." | Log
        }
    }
    
    $BtnOpenFolder = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'Show in Explorer'
        Location = New-Object System.Drawing.Point(260, 185)
        BackColor = 'Yellow'
        Add_Click = {
            Invoke-Item $Path
        }
    }
    
    $BtnLaunchSKSEAE = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'SKSE (AE)'
        Location = New-Object System.Drawing.Point(20, 185)
        Add_Click = {
            Push-Location $env:SkyrimAEPath
            Start-Process ./SKSE64_loader.exe
            Pop-Location

            "- SKSE (AE) Launched." | Log
        }
    }
    if (!(Test-Path "$env:SkyrimAEPath/skse64_loader.exe" -PathType Leaf)) {
        $BtnLaunchSKSEAE.Enabled = $false
    }

    $BtnLaunchSKSESE = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'SKSE (SE)'
        Location = New-Object System.Drawing.Point(100, 185)
        Add_Click = {
            Push-Location $env:SkyrimSEPath
            Start-Process ./SKSE64_loader.exe
            Pop-Location

            "- SKSE (SE) Launched." | Log
        }
    }
    if (!(Test-Path "$env:SkyrimSEPath/skse64_loader.exe" -PathType Leaf)) {
        $BtnLaunchSKSESE.Enabled = $false
    }
 
    $BtnLaunchSKSEVR = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'SKSE (VR)'
        Location = New-Object System.Drawing.Point(180, 185)
        Add_Click = {
            Push-Location $env:SkyrimVRPath
            Start-Process ./SKSE64_loader.exe
            Pop-Location

            "- SKSE (VR) Launched." | Log
        }
    }
    if (!(Test-Path "$env:SkyrimVRPath/skse64_loader.exe" -PathType Leaf)) {
        $BtnLaunchSKSEVR.Enabled = $false
    }
    
    $BtnBuildPapyrus = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'Build Papyrus'
        Location = New-Object System.Drawing.Point(20, 240)
        Add_Click = {
            $BtnBuildPapyrus.Text = 'Compiling...'
            
            $Invocation = "`"$($env:SkyrimSEPath)/Papyrus Compiler/PapyrusCompiler.exe`" `"$PSScriptRoot/Scripts/Source`" -f=`"$env:SkyrimSEPath/Papyrus Compiler/TESV_Papyrus_Flags.flg`" -i=`"$env:SkyrimSEPath/Data/Scripts/Source;$PSScriptRoot/Scripts;$PSScriptRoot/Scripts/Source`" -o=`"$PSScriptRoot/Scripts`" -a -op -enablecache -t=`"4`""
            Start-Process cmd.exe -ArgumentList "/k $Invocation && pause && exit"
            
            $BtnBuildPapyrus.Text = 'Build Papyrus'
        }
    }
    
    $BtnChangeVersion = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'Version'
        Location = New-Object System.Drawing.Point(100, 240)
        Add_Click = {
            $NewVersion = $null
            while ($OldVersion -and !$NewVersion) {
                $NewVersion = [Microsoft.VisualBasic.Interaction]::InputBox("Input the new versioning for $Project", 'Versioning', $OldVersion)
            }
            $ProjectCMake = $ProjectCMake -replace "VERSION\s$OldVersion", "VERSION $NewVersion"
            $vcpkg.'version-string' = $NewVersion

            [IO.File]::WriteAllText("$PSScriptRoot/CMakeLists.txt", $ProjectCMake)
            $vcpkg = $vcpkg | ConvertTo-Json -Depth 9
            [IO.File]::WriteAllText("$PSScriptRoot/vcpkg.json", $vcpkg)


            "- Version changed $OldVersion -> $NewVersion" | Log
            $OldVersion = $NewVersion
        }
    }
    
    $BtnPublish = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'Publish Mod'
        Location = New-Object System.Drawing.Point(180, 240)
        Add_Click = {
            $BtnPublish.Text = 'Zipping...'

            Copy-Mod "$PSScriptRoot/Tmp/Data"
            Compress-Archive "$PSScriptRoot/Tmp/Data/*" "$Path/$($Project)-$(($OldVersion).Replace('.', '-'))" -Force
            Remove-Item "$PSScriptRoot/Tmp" -Recurse -Force -Confirm:$false -ErrorAction:SilentlyContinue | Out-Null
            Invoke-Item $Path

            "- Mod files zipped & ready to go." | Log
            $BtnPublish.Text = 'Publish Mod'
        }
    }
    
    
    $BtnExit = New-Object System.Windows.Forms.Button -Property @{
        ClientSize = '70, 50'
        Text = 'Exit'
        Location = New-Object System.Drawing.Point(260, 240)
        Add_Click = {
            $MsgBox.Close()
        }
    }
                
    $MsgBox.Controls.Add($Message)
    $MsgBox.Controls.Add($BtnCopyData)
    $MsgBox.Controls.Add($BtnCopyMO2)
    $MsgBox.Controls.Add($BtnRemoveData)
    $MsgBox.Controls.Add($BtnOpenFolder)
    $MsgBox.Controls.Add($BtnExit)
    $MsgBox.Controls.Add($BtnBuildPapyrus)
    $MsgBox.Controls.Add($BtnChangeVersion)
    $MsgBox.Controls.Add($BtnPublish)
    $MsgBox.Controls.Add($BtnLaunchSKSEAE)
    $MsgBox.Controls.Add($BtnLaunchSKSESE)
    $MsgBox.Controls.Add($BtnLaunchSKSEVR)
    
    "- [$Project - $OldVersion] has been built." | Log
    $MsgBox.ShowDialog() | Out-Null
    Exit
}


# @@SOURCEGEN
if ($Mode -eq 'SOURCEGEN') {
    Write-Host "`tGenerating CMake sourcelist..."
    Remove-Item "$Path/sourcelist.cmake" -Force -Confirm:$false -ErrorAction Ignore

    $generated = 'set(SOURCES'
    $generated += $PSScriptRoot | Resolve-Files
    if ($Path) {
        $generated += $Path | Resolve-Files
    }
    $generated += "`n)"
    [IO.File]::WriteAllText("$Path/sourcelist.cmake", $generated)
}


# @@DISTRIBUTE
if ($Mode -eq 'DISTRIBUTE') { # update script to every project
    Get-ChildItem "$PSScriptRoot/*/*" -Directory | Where-Object {
        $_.Name -notin @('vcpkg', 'Build', '.git', '.vs') -and
        (Test-Path "$_/CMakeLists.txt" -PathType Leaf)
    } | ForEach-Object {
        Write-Host "`tUpdated <$_>"
        Robocopy.exe "$PSScriptRoot" "$_" '!Update.ps1' /MT /NJS /NFL /NDL /NJH | Out-Null
    }
}





# SIG # Begin signature block
# MIIbmwYJKoZIhvcNAQcCoIIbjDCCG4gCAQExCzAJBgUrDgMCGgUAMGkGCisGAQQB
# gjcCAQSgWzBZMDQGCisGAQQBgjcCAR4wJgIDAQAABBAfzDtgWUsITrck0sYpfvNR
# AgEAAgEAAgEAAgEAAgEAMCEwCQYFKw4DAhoFAAQU+7ayMKyy2DlXF4nohgd21MSz
# 54+gghYRMIIDBjCCAe6gAwIBAgIQEYQblqraVKtHgiCELkq6EzANBgkqhkiG9w0B
# AQsFADAbMRkwFwYDVQQDDBBES1NjcmlwdFNlbGZDZXJ0MB4XDTIyMDkxMDE3MTU0
# OVoXDTIzMDkxMDE3MzU0OVowGzEZMBcGA1UEAwwQREtTY3JpcHRTZWxmQ2VydDCC
# ASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMOfk79wlP6LSc6fa4kjex2C
# uoDj5EBz480/0gJRJ6KzCYHwdDdtb6CO/09zCoGnzwsRzZRjszLRT1MCKyAN28HZ
# ls6zevIqwN2MweRdG6GFbGl+gsFqMz/vNFhLoOxyjJkjNbqTBJzb//TzQ+7fMB8J
# 1EvWL2H2kKg+/iHSZ6sVHpuXXk7+WUj415bLIeRIk/7vvyP6dG46+NQaghd2FhVu
# BC6HZtvur/3dI5FQPXGT3UMi94kfnyPnxIhJ8pDNxtHmitFp+1j1EhA1czYx3nNU
# eUFlC8G0tjul3eUohv7q8yxNLgTb1vCCzdHsrFDBSfoS3q81OhzJo0KKx0jcYKEC
# AwEAAaNGMEQwDgYDVR0PAQH/BAQDAgeAMBMGA1UdJQQMMAoGCCsGAQUFBwMDMB0G
# A1UdDgQWBBQjwX2C95xsboK+DiU4y9H4vj6WsTANBgkqhkiG9w0BAQsFAAOCAQEA
# hn5/V+6i0sgIlcg/LuvgzGtuq/xQgHZ00AHpAwbBeDYtipVrzs4Cj2IllMpxsNL6
# NdtFPADRfZqFO4fBmLT3bmEAcXnda9U+Kzxvd+D2BSI1vvmbZSDXcnjp0GCLk9pR
# hQtUmayQ8eaAmZ5bCzFaeB5IekP4tPZXXr7PJFUJgjJ73FIB5ew1TWfhLps3A6KT
# rmj8ypjmbElzVZzF3DZgJkMIfq2ONJarQJ68fVOAoenyhUSJiXJbNHOM4gY5tEoq
# K4nsgUxSzFFvwvv8/n2qWorHdCUVxBGtJoGv6puxP8EAcLKQBWqkzQpSW79jM9Gw
# gFF2FlOd/hmM/nHhdcXcjzCCBY0wggR1oAMCAQICEA6bGI750C3n79tQ4ghAGFow
# DQYJKoZIhvcNAQEMBQAwZTELMAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0
# IEluYzEZMBcGA1UECxMQd3d3LmRpZ2ljZXJ0LmNvbTEkMCIGA1UEAxMbRGlnaUNl
# cnQgQXNzdXJlZCBJRCBSb290IENBMB4XDTIyMDgwMTAwMDAwMFoXDTMxMTEwOTIz
# NTk1OVowYjELMAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcG
# A1UECxMQd3d3LmRpZ2ljZXJ0LmNvbTEhMB8GA1UEAxMYRGlnaUNlcnQgVHJ1c3Rl
# ZCBSb290IEc0MIICIjANBgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAv+aQc2je
# u+RdSjwwIjBpM+zCpyUuySE98orYWcLhKac9WKt2ms2uexuEDcQwH/MbpDgW61bG
# l20dq7J58soR0uRf1gU8Ug9SH8aeFaV+vp+pVxZZVXKvaJNwwrK6dZlqczKU0RBE
# EC7fgvMHhOZ0O21x4i0MG+4g1ckgHWMpLc7sXk7Ik/ghYZs06wXGXuxbGrzryc/N
# rDRAX7F6Zu53yEioZldXn1RYjgwrt0+nMNlW7sp7XeOtyU9e5TXnMcvak17cjo+A
# 2raRmECQecN4x7axxLVqGDgDEI3Y1DekLgV9iPWCPhCRcKtVgkEy19sEcypukQF8
# IUzUvK4bA3VdeGbZOjFEmjNAvwjXWkmkwuapoGfdpCe8oU85tRFYF/ckXEaPZPfB
# aYh2mHY9WV1CdoeJl2l6SPDgohIbZpp0yt5LHucOY67m1O+SkjqePdwA5EUlibaa
# RBkrfsCUtNJhbesz2cXfSwQAzH0clcOP9yGyshG3u3/y1YxwLEFgqrFjGESVGnZi
# fvaAsPvoZKYz0YkH4b235kOkGLimdwHhD5QMIR2yVCkliWzlDlJRR3S+Jqy2QXXe
# eqxfjT/JvNNBERJb5RBQ6zHFynIWIgnffEx1P2PsIV/EIFFrb7GrhotPwtZFX50g
# /KEexcCPorF+CiaZ9eRpL5gdLfXZqbId5RsCAwEAAaOCATowggE2MA8GA1UdEwEB
# /wQFMAMBAf8wHQYDVR0OBBYEFOzX44LScV1kTN8uZz/nupiuHA9PMB8GA1UdIwQY
# MBaAFEXroq/0ksuCMS1Ri6enIZ3zbcgPMA4GA1UdDwEB/wQEAwIBhjB5BggrBgEF
# BQcBAQRtMGswJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBD
# BggrBgEFBQcwAoY3aHR0cDovL2NhY2VydHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0
# QXNzdXJlZElEUm9vdENBLmNydDBFBgNVHR8EPjA8MDqgOKA2hjRodHRwOi8vY3Js
# My5kaWdpY2VydC5jb20vRGlnaUNlcnRBc3N1cmVkSURSb290Q0EuY3JsMBEGA1Ud
# IAQKMAgwBgYEVR0gADANBgkqhkiG9w0BAQwFAAOCAQEAcKC/Q1xV5zhfoKN0Gz22
# Ftf3v1cHvZqsoYcs7IVeqRq7IviHGmlUIu2kiHdtvRoU9BNKei8ttzjv9P+Aufih
# 9/Jy3iS8UgPITtAq3votVs/59PesMHqai7Je1M/RQ0SbQyHrlnKhSLSZy51PpwYD
# E3cnRNTnf+hZqPC/Lwum6fI0POz3A8eHqNJMQBk1RmppVLC4oVaO7KTVPeix3P0c
# 2PR3WlxUjG/voVA9/HYJaISfb8rbII01YBwCA8sgsKxYoA5AY8WYIsGyWfVVa88n
# q2x2zm8jLfR+cWojayL/ErhULSd+2DrZ8LaHlv1b0VysGMNNn3O3AamfV6peKOK5
# lDCCBq4wggSWoAMCAQICEAc2N7ckVHzYR6z9KGYqXlswDQYJKoZIhvcNAQELBQAw
# YjELMAkGA1UEBhMCVVMxFTATBgNVBAoTDERpZ2lDZXJ0IEluYzEZMBcGA1UECxMQ
# d3d3LmRpZ2ljZXJ0LmNvbTEhMB8GA1UEAxMYRGlnaUNlcnQgVHJ1c3RlZCBSb290
# IEc0MB4XDTIyMDMyMzAwMDAwMFoXDTM3MDMyMjIzNTk1OVowYzELMAkGA1UEBhMC
# VVMxFzAVBgNVBAoTDkRpZ2lDZXJ0LCBJbmMuMTswOQYDVQQDEzJEaWdpQ2VydCBU
# cnVzdGVkIEc0IFJTQTQwOTYgU0hBMjU2IFRpbWVTdGFtcGluZyBDQTCCAiIwDQYJ
# KoZIhvcNAQEBBQADggIPADCCAgoCggIBAMaGNQZJs8E9cklRVcclA8TykTepl1Gh
# 1tKD0Z5Mom2gsMyD+Vr2EaFEFUJfpIjzaPp985yJC3+dH54PMx9QEwsmc5Zt+Feo
# An39Q7SE2hHxc7Gz7iuAhIoiGN/r2j3EF3+rGSs+QtxnjupRPfDWVtTnKC3r07G1
# decfBmWNlCnT2exp39mQh0YAe9tEQYncfGpXevA3eZ9drMvohGS0UvJ2R/dhgxnd
# X7RUCyFobjchu0CsX7LeSn3O9TkSZ+8OpWNs5KbFHc02DVzV5huowWR0QKfAcsW6
# Th+xtVhNef7Xj3OTrCw54qVI1vCwMROpVymWJy71h6aPTnYVVSZwmCZ/oBpHIEPj
# Q2OAe3VuJyWQmDo4EbP29p7mO1vsgd4iFNmCKseSv6De4z6ic/rnH1pslPJSlREr
# WHRAKKtzQ87fSqEcazjFKfPKqpZzQmiftkaznTqj1QPgv/CiPMpC3BhIfxQ0z9JM
# q++bPf4OuGQq+nUoJEHtQr8FnGZJUlD0UfM2SU2LINIsVzV5K6jzRWC8I41Y99xh
# 3pP+OcD5sjClTNfpmEpYPtMDiP6zj9NeS3YSUZPJjAw7W4oiqMEmCPkUEBIDfV8j
# u2TjY+Cm4T72wnSyPx4JduyrXUZ14mCjWAkBKAAOhFTuzuldyF4wEr1GnrXTdrnS
# DmuZDNIztM2xAgMBAAGjggFdMIIBWTASBgNVHRMBAf8ECDAGAQH/AgEAMB0GA1Ud
# DgQWBBS6FtltTYUvcyl2mi91jGogj57IbzAfBgNVHSMEGDAWgBTs1+OC0nFdZEzf
# Lmc/57qYrhwPTzAOBgNVHQ8BAf8EBAMCAYYwEwYDVR0lBAwwCgYIKwYBBQUHAwgw
# dwYIKwYBBQUHAQEEazBpMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2Vy
# dC5jb20wQQYIKwYBBQUHMAKGNWh0dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNvbS9E
# aWdpQ2VydFRydXN0ZWRSb290RzQuY3J0MEMGA1UdHwQ8MDowOKA2oDSGMmh0dHA6
# Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydFRydXN0ZWRSb290RzQuY3JsMCAG
# A1UdIAQZMBcwCAYGZ4EMAQQCMAsGCWCGSAGG/WwHATANBgkqhkiG9w0BAQsFAAOC
# AgEAfVmOwJO2b5ipRCIBfmbW2CFC4bAYLhBNE88wU86/GPvHUF3iSyn7cIoNqilp
# /GnBzx0H6T5gyNgL5Vxb122H+oQgJTQxZ822EpZvxFBMYh0MCIKoFr2pVs8Vc40B
# IiXOlWk/R3f7cnQU1/+rT4osequFzUNf7WC2qk+RZp4snuCKrOX9jLxkJodskr2d
# fNBwCnzvqLx1T7pa96kQsl3p/yhUifDVinF2ZdrM8HKjI/rAJ4JErpknG6skHibB
# t94q6/aesXmZgaNWhqsKRcnfxI2g55j7+6adcq/Ex8HBanHZxhOACcS2n82HhyS7
# T6NJuXdmkfFynOlLAlKnN36TU6w7HQhJD5TNOXrd/yVjmScsPT9rp/Fmw0HNT7ZA
# myEhQNC3EyTN3B14OuSereU0cZLXJmvkOHOrpgFPvT87eK1MrfvElXvtCl8zOYdB
# eHo46Zzh3SP9HSjTx/no8Zhf+yvYfvJGnXUsHicsJttvFXseGYs2uJPU5vIXmVnK
# cPA3v5gA3yAWTyf7YGcWoWa63VXAOimGsJigK+2VQbc61RWYMbRiCQ8KvYHZE/6/
# pNHzV9m8BPqC3jLfBInwAM1dwvnQI38AC+R2AibZ8GV2QqYphwlHK+Z/GqSFD/yY
# lvZVVCsfgPrA8g4r5db7qS9EFUrnEw4d2zc4GqEr9u3WfPwwggbAMIIEqKADAgEC
# AhAMTWlyS5T6PCpKPSkHgD1aMA0GCSqGSIb3DQEBCwUAMGMxCzAJBgNVBAYTAlVT
# MRcwFQYDVQQKEw5EaWdpQ2VydCwgSW5jLjE7MDkGA1UEAxMyRGlnaUNlcnQgVHJ1
# c3RlZCBHNCBSU0E0MDk2IFNIQTI1NiBUaW1lU3RhbXBpbmcgQ0EwHhcNMjIwOTIx
# MDAwMDAwWhcNMzMxMTIxMjM1OTU5WjBGMQswCQYDVQQGEwJVUzERMA8GA1UEChMI
# RGlnaUNlcnQxJDAiBgNVBAMTG0RpZ2lDZXJ0IFRpbWVzdGFtcCAyMDIyIC0gMjCC
# AiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAM/spSY6xqnya7uNwQ2a26Ho
# FIV0MxomrNAcVR4eNm28klUMYfSdCXc9FZYIL2tkpP0GgxbXkZI4HDEClvtysZc6
# Va8z7GGK6aYo25BjXL2JU+A6LYyHQq4mpOS7eHi5ehbhVsbAumRTuyoW51BIu4hp
# DIjG8b7gL307scpTjUCDHufLckkoHkyAHoVW54Xt8mG8qjoHffarbuVm3eJc9S/t
# jdRNlYRo44DLannR0hCRRinrPibytIzNTLlmyLuqUDgN5YyUXRlav/V7QG5vFqia
# nJVHhoV5PgxeZowaCiS+nKrSnLb3T254xCg/oxwPUAY3ugjZNaa1Htp4WB056PhM
# kRCWfk3h3cKtpX74LRsf7CtGGKMZ9jn39cFPcS6JAxGiS7uYv/pP5Hs27wZE5FX/
# NurlfDHn88JSxOYWe1p+pSVz28BqmSEtY+VZ9U0vkB8nt9KrFOU4ZodRCGv7U0M5
# 0GT6Vs/g9ArmFG1keLuY/ZTDcyHzL8IuINeBrNPxB9ThvdldS24xlCmL5kGkZZTA
# WOXlLimQprdhZPrZIGwYUWC6poEPCSVT8b876asHDmoHOWIZydaFfxPZjXnPYsXs
# 4Xu5zGcTB5rBeO3GiMiwbjJ5xwtZg43G7vUsfHuOy2SJ8bHEuOdTXl9V0n0ZKVkD
# Tvpd6kVzHIR+187i1Dp3AgMBAAGjggGLMIIBhzAOBgNVHQ8BAf8EBAMCB4AwDAYD
# VR0TAQH/BAIwADAWBgNVHSUBAf8EDDAKBggrBgEFBQcDCDAgBgNVHSAEGTAXMAgG
# BmeBDAEEAjALBglghkgBhv1sBwEwHwYDVR0jBBgwFoAUuhbZbU2FL3MpdpovdYxq
# II+eyG8wHQYDVR0OBBYEFGKK3tBh/I8xFO2XC809KpQU31KcMFoGA1UdHwRTMFEw
# T6BNoEuGSWh0dHA6Ly9jcmwzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydFRydXN0ZWRH
# NFJTQTQwOTZTSEEyNTZUaW1lU3RhbXBpbmdDQS5jcmwwgZAGCCsGAQUFBwEBBIGD
# MIGAMCQGCCsGAQUFBzABhhhodHRwOi8vb2NzcC5kaWdpY2VydC5jb20wWAYIKwYB
# BQUHMAKGTGh0dHA6Ly9jYWNlcnRzLmRpZ2ljZXJ0LmNvbS9EaWdpQ2VydFRydXN0
# ZWRHNFJTQTQwOTZTSEEyNTZUaW1lU3RhbXBpbmdDQS5jcnQwDQYJKoZIhvcNAQEL
# BQADggIBAFWqKhrzRvN4Vzcw/HXjT9aFI/H8+ZU5myXm93KKmMN31GT8Ffs2wklR
# LHiIY1UJRjkA/GnUypsp+6M/wMkAmxMdsJiJ3HjyzXyFzVOdr2LiYWajFCpFh0qY
# QitQ/Bu1nggwCfrkLdcJiXn5CeaIzn0buGqim8FTYAnoo7id160fHLjsmEHw9g6A
# ++T/350Qp+sAul9Kjxo6UrTqvwlJFTU2WZoPVNKyG39+XgmtdlSKdG3K0gVnK3br
# /5iyJpU4GYhEFOUKWaJr5yI+RCHSPxzAm+18SLLYkgyRTzxmlK9dAlPrnuKe5NMf
# hgFknADC6Vp0dQ094XmIvxwBl8kZI4DXNlpflhaxYwzGRkA7zl011Fk+Q5oYrsPJ
# y8P7mxNfarXH4PMFw1nfJ2Ir3kHJU7n/NBBn9iYymHv+XEKUgZSCnawKi8ZLFUrT
# mJBFYDOA4CPe+AOk9kVH5c64A0JH6EE2cXet/aLol3ROLtoeHYxayB6a1cLwxiKo
# T5u92ByaUcQvmvZfpyeXupYuhVfAYOd4Vn9q78KVmksRAsiCnMkaBXy6cbVOepls
# 9Oie1FqYyJ+/jbsYXEP10Cro4mLueATbvdH7WwqocH7wl4R44wgDXUcsY6glOJcB
# 0j862uXl9uab3H4szP8XTE0AotjWAQ64i+7m4HJViSwnGWH2dwGMMYIE9DCCBPAC
# AQEwLzAbMRkwFwYDVQQDDBBES1NjcmlwdFNlbGZDZXJ0AhARhBuWqtpUq0eCIIQu
# SroTMAkGBSsOAwIaBQCgeDAYBgorBgEEAYI3AgEMMQowCKACgAChAoAAMBkGCSqG
# SIb3DQEJAzEMBgorBgEEAYI3AgEEMBwGCisGAQQBgjcCAQsxDjAMBgorBgEEAYI3
# AgEVMCMGCSqGSIb3DQEJBDEWBBSYq1cGXD3doDR6DzKlgyyWS5tuZjANBgkqhkiG
# 9w0BAQEFAASCAQBQOuP6i068+ql+YpgIuL+8l4ck7jZiLyzo7g2/sicOSVaDvtR5
# Jqq2Dv9XGMVfFDWWxcDwJ0qdoMWsp0o+Ff+LnG01epiXCf6AlmY+9QbSkLBknKF8
# 8fLzC/U4K3B2rIWg9ope3YJGRykcRR/6B3yUqqlF6fNNNk/fBM1Jb54IHMksNXwG
# p+gQPcNnk/kod61CoLCT414w6AZ5PL9ppzS4CabTg0/Ip+u2cyWze6C0fXAmabKd
# yGU4YH5xRLKc4OtDBUO2Tf+nZ5EzWofYKZ8VXUmBIGFh2C5jZvK7ULzCF9iX9jOw
# AlryXcqKzLrRgqZbsmGsoGLZyvy9Z6OIR7eOoYIDIDCCAxwGCSqGSIb3DQEJBjGC
# Aw0wggMJAgEBMHcwYzELMAkGA1UEBhMCVVMxFzAVBgNVBAoTDkRpZ2lDZXJ0LCBJ
# bmMuMTswOQYDVQQDEzJEaWdpQ2VydCBUcnVzdGVkIEc0IFJTQTQwOTYgU0hBMjU2
# IFRpbWVTdGFtcGluZyBDQQIQDE1pckuU+jwqSj0pB4A9WjANBglghkgBZQMEAgEF
# AKBpMBgGCSqGSIb3DQEJAzELBgkqhkiG9w0BBwEwHAYJKoZIhvcNAQkFMQ8XDTIz
# MDIxMjExMjYxOFowLwYJKoZIhvcNAQkEMSIEIF/ONVk6oA1q9vxnxBciceMiZc6N
# RvOqy6257D8LyhxVMA0GCSqGSIb3DQEBAQUABIICAJMky02wnZWdcIrIeNWaXuVK
# LGgarHZd2HikPARnA0+6N4wbvgnXbvEw9ysCm3AA3p4AY7vQlR/+VuJF7vu+SMrr
# DFIMT7UAZQWovOMxZKPE4mtN32Ro1Pz8e6kugs3kB/iJHDVa8IvZ6Fo3Tm9lUrEe
# NT4MDuID/+PJhIM+O+oP6g4ezoLRbPddWAPA1g7Rfmyy7q6Ak7qAubMmPTvojYx9
# fIwtZYY3FHe7V7E9T0/VLlkeFuqQiR9iX/6KW0CQwSdu6gDeQAEgbMKBjlTyMWth
# UfbYqW9wbydS7edmLHsnEpJIoyyiPykw4+TyD9Fn1djTBFWauuW6qDy68jRN9AmI
# Rt1kX2KCxp2wA+XXwk/fhZ57hsC5TH0xeGeN8tPwcXoyggaFwpFTQXh2JvyFMm1Y
# BWH6J7S+Rv++EwwlFu+aHH7cmmga+CjWKvt2lmNXA8ro6SD+Ux4HvjG9rKOt8IVs
# IHVw+dyHVj/diBFpdYayMY9CZiInBuoPkPFRZljev9i6dXbpK4WX2VY/9Z6udbRA
# wjBDO4//xg0lN9CsvWe3Wn62+/Ii5HMEzTBzb+8TfvHkt9YoaDX3tyuz7ltovy41
# I0wppbcU4AKwBjREyN/QnQ7QxjOFJG+hcEyPUupC4gxcnUqeJnAuBV5mbn9r0wnH
# gi4XKW3ULJKEhVjdWtxJ
# SIG # End signature block
