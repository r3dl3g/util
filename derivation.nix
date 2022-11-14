# https://discourse.nixos.org/t/how-to-use-a-nix-derivation-from-a-local-folder/5498/4

{ stdenv, lib, pkgs } :

stdenv.mkDerivation {
  pname = "util";
  version = "1.1.7";

  src = ./.;

  nativeBuildInputs = with pkgs; [ cmake ];

  enableParallelBuilding = true;

  outputs = [ "dev" "out" ];

  cmakeFlags = [
    "-DUTIL_CONFIG_INSTALL=ON"
    "-DUTIL_BUILD_STATIC_MODULE_LIB=ON"
    "-DUTIL_TESTS=ON"
  ];

  doCheck = true;

  checkPhase = ''
    pushd tests
    echo WorkingDir: $PWD
    echo ls -la
    ctest .
    popd
  '';

  meta = with lib; {
    description = "A c++ utility library";
    homepage = "https://github.com/r3dl3g/util";
    changelog = "https://github.com/r3dl3g/util/releases/tag/v${version}";
    license = licenses.mit;
    maintainers = with maintainers; [ r3dl3g ];
    platforms = with platforms; linux;
  };
}
