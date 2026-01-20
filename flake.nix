{
  description = "CLI Tool for managing C++ projects";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
    pkgs = import nixpkgs {inherit system;};
    stdenv = pkgs.gcc15Stdenv;
  in {
    packages.${system} = rec {
      cpp-cli = stdenv.mkDerivation {
        pname = "cpp-cli";
        version = "1.0.0";

        src = pkgs.lib.cleanSource ./.;

        nativeBuildInputs = with pkgs; [
          cmake
          ninja
          pkg-config
        ];

        buildInputs = with pkgs; [
          yaml-cpp
        ];

        cmakeFlags = [
          "-G Ninja"
          "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
        ];

        installPhase = ''
          mkdir -p $out/bin
          cp cpp-cli $out/bin/
        '';
      };
      default = cpp-cli;
    };

    apps.${system}.default = {
      type = "app";
      program = "${self.packages.${system}.cpp-cli}/bin/cpp-cli";
    };

    devShells.${system}.default = pkgs.mkShell.override {inherit stdenv;} {
      packages = with pkgs; [
        cmake
        ninja
        pkg-config
        yaml-cpp
        llvmPackages_18.clang-tools
        gnumake
      ];

      shellHook = ''
        export CPLUS_INCLUDE_PATH="${pkgs.yaml-cpp}/include:$CPLUS_INCLUDE_PATH"
        exec zsh
      '';
    };
  };
}
