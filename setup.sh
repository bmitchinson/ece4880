#!/usr/bin/env bash
# Author: "abpwrs"
# Date: 20190915

# script:
cd `dirname ${0}`
echo "git secret hide" >> ./.git/hooks/pre-commit
echo "git secret reveal" >> ./.git/hooks/post-merge
chmod 700 ./.git/hooks/pre-commit ./.git/hooks/post-merge

