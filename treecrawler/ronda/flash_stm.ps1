# flash_stm.ps1 - PowerShell Script for Remote STM32 Flashing

# --- CONFIGURATION ---
# IMPORTANT: Update these variables for your specific setup

# Desktop Path to your compiled binary file (Windows path style is assumed)
$BINARY_PATH = ".\.pio\build\nucleo_f401re\firmware.bin"

# Jetson connection details
$JETSON_USER = "cantinabot"             # Your username on the Jetson
$JETSON_IP = "192.168.1.233"            # The Jetson's IP address

# Remote path on the Jetson where the binary will be temporarily stored
# NOTE: Linux path format is used for the remote paths
$REMOTE_DIR = "/home/$JETSON_USER/Documents/ronda/stm_flash"
$REMOTE_FILE_NAME = "firmware.bin"
$REMOTE_FILE_PATH = "$REMOTE_DIR/$REMOTE_FILE_NAME"

# OpenOCD configuration files (adjust these to match your programmer and chip)
$OPENOCD_INTERFACE = "interface/stlink.cfg"
$OPENOCD_TARGET = "target/stm32f4x.cfg"

# --- EXECUTION ---

# 1. Check if the local binary file exists
if (-not (Test-Path $BINARY_PATH)) {
    Write-Error "Error: Binary file not found at $BINARY_PATH"
    Write-Host "Please ensure you have run 'pio run' on your desktop first."
    exit 1
}

Write-Host "--- Starting Remote STM32 Flash ---"

# 2. Securely transfer the binary file to the Jetson using scp
Write-Host "1. Transferring $BINARY_PATH to Jetson at $JETSON_IP..."

# The scp command is executed directly.
& scp $BINARY_PATH "${JETSON_USER}@${JETSON_IP}:${REMOTE_DIR}/"

if ($LASTEXITCODE -ne 0) {
    Write-Error "Error: SCP failed. Check connectivity and SSH/SCP clients."
    exit 1
}

# 3. Execute the flashing command remotely via SSH
# NOTE: 'sudo' is included here. This requires either UDEV rules to avoid sudo, 
# or passwordless sudo to be configured on the Jetson for the 'openocd' command.
# For STM32F401RE: explicitly specify flash address (0x08000000) to avoid "no flash bank found" warning
$FLASH_COMMAND = "sudo openocd -f $OPENOCD_INTERFACE -f $OPENOCD_TARGET -c 'program $REMOTE_FILE_PATH 0x08000000 verify reset exit'"

Write-Host "2. Executing flash command on Jetson..."

# SSH command is executed directly with -t flag to allocate pseudo-terminal for sudo password prompt.
& ssh -t "$JETSON_USER@$JETSON_IP" $FLASH_COMMAND

if ($LASTEXITCODE -ne 0) {
    Write-Warning "Warning: Flashing may have failed. Review OpenOCD output above for details."
}

Write-Host "--- Flash Process Complete ---"