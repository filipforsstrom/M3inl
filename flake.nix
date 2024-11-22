{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
    arduino-nix.url = "github:bouk/arduino-nix";
    arduino-library-index = {
      url = "github:bouk/arduino-indexes/library_index";
      flake = false;
    };
    arduino-package-index = {
      url = "github:bouk/arduino-indexes/package_index";
      flake = false;
    };
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
    arduino-nix,
    arduino-package-index,
    arduino-library-index,
  } @ attrs: let
    overlays = [
      (arduino-nix.overlay)
      (arduino-nix.mkArduinoPackageOverlay ./package_index.json)
      (arduino-nix.mkArduinoLibraryOverlay (arduino-library-index + "/library_index.json"))
    ];
  in (flake-utils.lib.eachDefaultSystem (
    system: let
      pkgs = (import nixpkgs) {
        inherit system overlays;
      };
    in rec {
      packages.arduino-cli = pkgs.wrapArduinoCLI {
        libraries = [
          (arduino-nix.latestVersion pkgs.arduinoLibraries."InputDebounce")
        ];

        packages = with pkgs.arduinoPackages; [
          # platforms.arduino.avr."1.8.6"
          platforms.arduino.renesas_uno."1.2.2"
        ];
      };

      devShell = pkgs.mkShell {
        packages = [
          packages.arduino-cli
        ];
        buildInputs = with pkgs; [
          # gcc
          # make
        ];
        shellHook = ''
          export COMPILER_PATH="${packages.arduino-cli.passthru.dataPath}/packages/arduino/tools/arm-none-eabi-gcc/7-2017q4/bin/"
          export PACKAGES_PATH="${packages.arduino-cli.passthru.dataPath}/packages"
          export LIBRARIES_PATH="${packages.arduino-cli.passthru.userPath}/libraries"
          export INO_PATH="./M3inl.ino"
          export DEVICE_PORT="/dev/serial/by-id/usb-Arduino_UNO_WiFi_R4_CMSIS-DAP_48CA435E4530-if01"
        '';
      };
    }
  ));
}
