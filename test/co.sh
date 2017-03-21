#! /bin/bash
rm -rf ./app_flow
mkdir ./app_flow
svn co https://tc-svn.tencent.com/isd/isd_happyfarm_rep/isd_qzoneappbase_proj/trunk/php/app_flow ./app_flow
rm -rf ./app_flow/app_views
rm -rf ./app_flow/sql
find . -name "*svn*"|xargs rm -rf
tar cvfz app_flow.tar.gz ./app_flow
