#!/bin/bash

cp /home/asibilev/build/SalixEDA/Desktop_Qt_6_6_3_GCC_64bit/Release/SalixEDA /home/asibilev/SalixEDAInstall/SalixEDAint

zip -y linCore.zip ./uninstall
zip -y linCore.zip ./icon.png
zip -y linCore.zip ./SalixEDAint
zip -y linCore.zip ./*.qm

exit 1
#scp linCore.zip root@178.212.13.179:/srv/web/salixeda.org/data/

rm compStore.zip
zip -ry compStore.zip ./compStore/

rm guide.zip
zip -ry guide.zip ./guide/

rm linLibs.zip
zip -ry linLibs.zip ./lib/

rm linPlugins.zip
zip -ry linPlugins.zip ./plugins/
