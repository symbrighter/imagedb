#!/bin/sh

test_description='Test imagedb service'

. `dirname $0`/sharness.sh

# TODO:
export PATH=/home/dahn/workspace/imagedb/t/../imagelogue/cli:${PATH}

test_expect_success 'imagedb-server: valid sqlite3 db file must exist' '
	test_must_fail imagedb-server > out 2>&1
'

test_done
