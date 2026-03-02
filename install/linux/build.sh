#!/bin/bash

~/Qt/Tools/QtInstallerFramework/4.6/bin/archivegen  packages/SaliEDA/data/salix ~/SalixEDAinstall/*

~/Qt/Tools/QtInstallerFramework/4.6/bin/binarycreator -c config/config.xml -p packages SalixEDA_3_0_Linux_x86_64_Install

rm packages/SalixEDA/data/salix.7z
