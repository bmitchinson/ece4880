#!/usr/bin/env bash
# Author: "abpwrs"
# Date: 20190915

# script:
echo "git secret hide" >> ./.git/hooks/pre-commit
echo "git secret reveal" >> ./.git/hooks/post-merge
