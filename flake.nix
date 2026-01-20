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
        src = ./.;

        nativeBuildInputs = with pkgs; [
          cmake
          ninja
          pkg-config
        ];

        buildInputs = with pkgs; [
          yaml-cpp
        ];

        cmakeFlags = ["-G Ninja"];

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
        gnumake
        pkg-config
        yaml-cpp
        llvmPackages_18.clang-tools
        ninja
      ];
    };
  };
}
