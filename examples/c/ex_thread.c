/*
 * ex_access.c Copyright (c) 2010 WiredTiger, Inc.  All rights reserved.
 *
 * This is an example demonstrating how to create and access a simple table.
 */

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include <wiredtiger.h>

const char *home = "WT_TEST";

WT_CONNECTION *conn;

void *scan_thread(void *arg) {
	WT_SESSION *session;
	WT_CURSOR *cursor;
	const char *key, *value;
	int ret;

	ret = conn->open_session(conn, NULL, NULL, &session);
	ret = session->open_cursor(session, "table:access", NULL, &cursor);

	/* Show all records. */
	for (ret = cursor->first(cursor);
	    ret == 0;
	    ret = cursor->next(cursor)) {
		ret = cursor->get_key(cursor, &key);
		ret = cursor->get_value(cursor, &value);

		printf("Got record: %s : %s\n", key, value);
	}

	return (arg);
}

int main()
{
	WT_SESSION *session;
	WT_CURSOR *cursor;
	int ret;

	if ((ret = wiredtiger_open(home, NULL, "create", &conn)) != 0)
		fprintf(stderr, "Error connecting to %s: %s\n",
		    home, wiredtiger_strerror(ret));
	/* Note: further error checking omitted for clarity. */

	ret = conn->open_session(conn, NULL, NULL, &session);
	ret = session->create_table(session, "access",
	    "key_format=S,value_format=S");
	ret = session->open_cursor(session, "table:access",
	    "overwrite", &cursor);
	ret = cursor->set_key(cursor, "key1");
	ret = cursor->set_value(cursor, "value1");
	ret = cursor->insert(cursor);
	ret = session->close(session, NULL);

	/* XXX start threads */
	/* XXX join threads */

	ret = conn->close(conn, NULL);

	return (ret);
}
