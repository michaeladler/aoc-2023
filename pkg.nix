{ stdenv, lib, meson, ninja, pkg-config, xxHash }:

stdenv.mkDerivation rec {
  pname = "aoc-2023";
  version = "0.1.1";

  src = ./.;

  nativeBuildInputs = [ meson ninja pkg-config ];

  buildInputs = [ xxHash ];

  NIX_LDFLAGS = "-lm";

}
