echo ======= New Prepare at $(date) ======= >> prepare.log
cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=../libs cmake -S. -Bbuild -DCMAKE_INSTALL_PREFIX=../libs -DUTIL_CONFIG_INSTALL=ON -DUTIL_BUILD_STATIC_MODULE_LIB=OFF -DUTIL_TESTS=OFF $* 2>&1 | tee -a prepare.log
