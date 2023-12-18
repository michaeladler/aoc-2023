{ stdenv, lib, meson, ninja, pkg-config, xxHash }:

stdenv.mkDerivation {
  pname = "aoc-2023";
  version = "0.1.0";

  src = ./.;

  nativeBuildInputs = [ meson ninja pkg-config ];

  buildInputs = [ xxHash ];

  # mesonFlags = [ "-Db_lto=true " ];
}
