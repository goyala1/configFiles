static xint proxy_get_caps( store_t *gctx )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	xint rv = ctx->real_driver->get_caps( ctx->real_store );
	debug( "%sCalled get_caps, ret=%#x\n", ctx->label, rv );
	return rv;
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, void *aux );
	void *callback_aux;
} connect_store_cmd_t;

static void
proxy_connect_store_cb( int sts, void *aux )
{
	connect_store_cmd_t *cmd = (connect_store_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter connect_store, sts=%d\n", cmd->gen.ctx->label, cmd->gen.tag, sts );
	cmd->callback( sts, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave connect_store\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_connect_store( store_t *gctx, void (*cb)( int sts, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	connect_store_cmd_t *cmd = (connect_store_cmd_t *)proxy_cmd_new( ctx, sizeof(connect_store_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	debug( "%s[% 2d] Enter connect_store\n", ctx->label, cmd->gen.tag );
	ctx->real_driver->connect_store( ctx->real_store, proxy_connect_store_cb, cmd );
	debug( "%s[% 2d] Leave connect_store\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void proxy_free_store( store_t *gctx )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	debug( "%sEnter free_store\n", ctx->label );
	ctx->real_driver->free_store( ctx->real_store );
	debug( "%sLeave free_store\n", ctx->label );
	proxy_store_deref( ctx );
}

static void proxy_cancel_store( store_t *gctx )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	debug( "%sEnter cancel_store\n", ctx->label );
	ctx->real_driver->cancel_store( ctx->real_store );
	debug( "%sLeave cancel_store\n", ctx->label );
	proxy_store_deref( ctx );
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, string_list_t *boxes, void *aux );
	void *callback_aux;
} list_store_cmd_t;

static void
proxy_list_store_cb( int sts, string_list_t *boxes, void *aux )
{
	list_store_cmd_t *cmd = (list_store_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter list_store\n", cmd->gen.ctx->label, cmd->gen.tag );
	if (sts == DRV_OK) {
		for (string_list_t *box = boxes; box; box = box->next)
			debug( "  %s\n", box->string );
	}
	cmd->callback( sts, boxes, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave list_store\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_list_store( store_t *gctx, int flags, void (*cb)( int sts, string_list_t *boxes, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	list_store_cmd_t *cmd = (list_store_cmd_t *)proxy_cmd_new( ctx, sizeof(list_store_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	debug( "%s[% 2d] Enter list_store, flags=%d\n", ctx->label, cmd->gen.tag, flags );
	ctx->real_driver->list_store( ctx->real_store, flags, proxy_list_store_cb, cmd );
	debug( "%s[% 2d] Leave list_store\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static int proxy_select_box( store_t *gctx, const char *name )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	debug( "%sEnter select_box, name=%s\n", ctx->label, name );
	int rv = ctx->real_driver->select_box( ctx->real_store, name );
	debug( "%sLeave select_box, ret=%d\n", ctx->label, rv );
	return rv;
}

static const char *proxy_get_box_path( store_t *gctx )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	const char *rv = ctx->real_driver->get_box_path( ctx->real_store );
	debug( "%sCalled get_box_path, ret=%s\n", ctx->label, rv );
	return rv;
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, void *aux );
	void *callback_aux;
} create_box_cmd_t;

static void
proxy_create_box_cb( int sts, void *aux )
{
	create_box_cmd_t *cmd = (create_box_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter create_box, sts=%d\n", cmd->gen.ctx->label, cmd->gen.tag, sts );
	cmd->callback( sts, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave create_box\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_create_box( store_t *gctx, void (*cb)( int sts, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	create_box_cmd_t *cmd = (create_box_cmd_t *)proxy_cmd_new( ctx, sizeof(create_box_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	debug( "%s[% 2d] Enter create_box\n", ctx->label, cmd->gen.tag );
	ctx->real_driver->create_box( ctx->real_store, proxy_create_box_cb, cmd );
	debug( "%s[% 2d] Leave create_box\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, int uidvalidity, void *aux );
	void *callback_aux;
} open_box_cmd_t;

static void
proxy_open_box_cb( int sts, int uidvalidity, void *aux )
{
	open_box_cmd_t *cmd = (open_box_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter open_box, sts=%d, uidvalidity=%d\n", cmd->gen.ctx->label, cmd->gen.tag, sts, uidvalidity );
	cmd->callback( sts, uidvalidity, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave open_box\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_open_box( store_t *gctx, void (*cb)( int sts, int uidvalidity, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	open_box_cmd_t *cmd = (open_box_cmd_t *)proxy_cmd_new( ctx, sizeof(open_box_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	debug( "%s[% 2d] Enter open_box\n", ctx->label, cmd->gen.tag );
	ctx->real_driver->open_box( ctx->real_store, proxy_open_box_cb, cmd );
	debug( "%s[% 2d] Leave open_box\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static int proxy_get_uidnext( store_t *gctx )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	int rv = ctx->real_driver->get_uidnext( ctx->real_store );
	debug( "%sCalled get_uidnext, ret=%d\n", ctx->label, rv );
	return rv;
}

static int proxy_confirm_box_empty( store_t *gctx )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	debug( "%sEnter confirm_box_empty\n", ctx->label );
	int rv = ctx->real_driver->confirm_box_empty( ctx->real_store );
	debug( "%sLeave confirm_box_empty, ret=%d\n", ctx->label, rv );
	return rv;
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, void *aux );
	void *callback_aux;
} delete_box_cmd_t;

static void
proxy_delete_box_cb( int sts, void *aux )
{
	delete_box_cmd_t *cmd = (delete_box_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter delete_box, sts=%d\n", cmd->gen.ctx->label, cmd->gen.tag, sts );
	cmd->callback( sts, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave delete_box\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_delete_box( store_t *gctx, void (*cb)( int sts, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	delete_box_cmd_t *cmd = (delete_box_cmd_t *)proxy_cmd_new( ctx, sizeof(delete_box_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	debug( "%s[% 2d] Enter delete_box\n", ctx->label, cmd->gen.tag );
	ctx->real_driver->delete_box( ctx->real_store, proxy_delete_box_cb, cmd );
	debug( "%s[% 2d] Leave delete_box\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static int proxy_finish_delete_box( store_t *gctx )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	debug( "%sEnter finish_delete_box\n", ctx->label );
	int rv = ctx->real_driver->finish_delete_box( ctx->real_store );
	debug( "%sLeave finish_delete_box, ret=%d\n", ctx->label, rv );
	return rv;
}

static xint proxy_prepare_load_box( store_t *gctx, xint opts )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	debug( "%sEnter prepare_load_box, opts=%#x\n", ctx->label, opts );
	xint rv = ctx->real_driver->prepare_load_box( ctx->real_store, opts );
	debug( "%sLeave prepare_load_box, ret=%#x\n", ctx->label, rv );
	return rv;
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, message_t *msgs, int total_msgs, int recent_msgs, void *aux );
	void *callback_aux;
} load_box_cmd_t;

static void
proxy_load_box_cb( int sts, message_t *msgs, int total_msgs, int recent_msgs, void *aux )
{
	load_box_cmd_t *cmd = (load_box_cmd_t *)aux;

	static char fbuf[as(Flags) + 1];
	debug( "%s[% 2d] Callback enter load_box, sts=%d, total=%d, recent=%d\n", cmd->gen.ctx->label, cmd->gen.tag, sts, total_msgs, recent_msgs );
	if (sts == DRV_OK) {
		for (message_t *msg = msgs; msg; msg = msg->next)
			debug( "  uid=%5u, flags=%4s, size=%6d, tuid=%." stringify(TUIDL) "s\n",
			       msg->uid, (msg->status & M_FLAGS) ? (proxy_make_flags( msg->flags, fbuf ), fbuf) : "?", msg->size, *msg->tuid ? msg->tuid : "?" );
	}
	cmd->callback( sts, msgs, total_msgs, recent_msgs, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave load_box\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_load_box( store_t *gctx, uint minuid, uint maxuid, uint newuid, uint seenuid, uint_array_t excs, void (*cb)( int sts, message_t *msgs, int total_msgs, int recent_msgs, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	load_box_cmd_t *cmd = (load_box_cmd_t *)proxy_cmd_new( ctx, sizeof(load_box_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	static char ubuf[12];
	debug( "%s[% 2d] Enter load_box, [%u,%s] (new >= %u, seen <= %u)\n", ctx->label, cmd->gen.tag, minuid, (maxuid == UINT_MAX) ? "inf" : (nfsnprintf( ubuf, sizeof(ubuf), "%u", maxuid ), ubuf), newuid, seenuid );
	if (excs.size) {
		debugn( "  excs:" );
		for (int t = 0; t < excs.size; t++)
			debugn( " %d", excs.data[t] );
		debug( "\n" );
	}
	ctx->real_driver->load_box( ctx->real_store, minuid, maxuid, newuid, seenuid, excs, proxy_load_box_cb, cmd );
	debug( "%s[% 2d] Leave load_box\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, void *aux );
	void *callback_aux;
	msg_data_t *data;
} fetch_msg_cmd_t;

static void
proxy_fetch_msg_cb( int sts, void *aux )
{
	fetch_msg_cmd_t *cmd = (fetch_msg_cmd_t *)aux;

	static char fbuf[as(Flags) + 1];
	proxy_make_flags( cmd->data->flags, fbuf );
	debug( "%s[% 2d] Callback enter fetch_msg, flags=%s, date=%ld, size=%d\n", cmd->gen.ctx->label, cmd->gen.tag, fbuf, cmd->data->date, cmd->data->len );
	if (sts == DRV_OK && (DFlags & DEBUG_DRV_ALL)) {
		printf( "%s=========\n", cmd->gen.ctx->label );
		fwrite( cmd->data->data, cmd->data->len, 1, stdout );
		printf( "%s=========\n", cmd->gen.ctx->label );
		fflush( stdout );
	}
	cmd->callback( sts, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave fetch_msg\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_fetch_msg( store_t *gctx, message_t *msg, msg_data_t *data, void (*cb)( int sts, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	fetch_msg_cmd_t *cmd = (fetch_msg_cmd_t *)proxy_cmd_new( ctx, sizeof(fetch_msg_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	cmd->data = data;
	debug( "%s[% 2d] Enter fetch_msg, uid=%u, want_flags=%s, want_date=%s\n", ctx->label, cmd->gen.tag, msg->uid, !(msg->status & M_FLAGS) ? "yes" : "no", data->date ? "yes" : "no" );
	ctx->real_driver->fetch_msg( ctx->real_store, msg, data, proxy_fetch_msg_cb, cmd );
	debug( "%s[% 2d] Leave fetch_msg\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, uint uid, void *aux );
	void *callback_aux;
} store_msg_cmd_t;

static void
proxy_store_msg_cb( int sts, uint uid, void *aux )
{
	store_msg_cmd_t *cmd = (store_msg_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter store_msg, sts=%d, u%duid\n", cmd->gen.ctx->label, cmd->gen.tag, sts, uid );
	cmd->callback( sts, uid, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave store_msg\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_store_msg( store_t *gctx, msg_data_t *data, int to_trash, void (*cb)( int sts, uint uid, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	store_msg_cmd_t *cmd = (store_msg_cmd_t *)proxy_cmd_new( ctx, sizeof(store_msg_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	static char fbuf[as(Flags) + 1];
	proxy_make_flags( data->flags, fbuf );
	debug( "%s[% 2d] Enter store_msg, flags=%s, date=%ld, size=%d, to_trash=%s\n", ctx->label, cmd->gen.tag, fbuf, data->date, data->len, to_trash ? "yes" : "no" );
	if (DFlags & DEBUG_DRV_ALL) {
		printf( "%s>>>>>>>>>\n", ctx->label );
		fwrite( data->data, data->len, 1, stdout );
		printf( "%s>>>>>>>>>\n", ctx->label );
		fflush( stdout );
	}
	ctx->real_driver->store_msg( ctx->real_store, data, to_trash, proxy_store_msg_cb, cmd );
	debug( "%s[% 2d] Leave store_msg\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, message_t *msgs, void *aux );
	void *callback_aux;
} find_new_msgs_cmd_t;

static void
proxy_find_new_msgs_cb( int sts, message_t *msgs, void *aux )
{
	find_new_msgs_cmd_t *cmd = (find_new_msgs_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter find_new_msgs, sts=%d\n", cmd->gen.ctx->label, cmd->gen.tag, sts );
	if (sts == DRV_OK) {
		for (message_t *msg = msgs; msg; msg = msg->next)
			debug( "  uid=%5u, tuid=%." stringify(TUIDL) "s\n", msg->uid, msg->tuid );
	}
	cmd->callback( sts, msgs, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave find_new_msgs\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_find_new_msgs( store_t *gctx, uint newuid, void (*cb)( int sts, message_t *msgs, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	find_new_msgs_cmd_t *cmd = (find_new_msgs_cmd_t *)proxy_cmd_new( ctx, sizeof(find_new_msgs_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	debug( "%s[% 2d] Enter find_new_msgs, u%dnewuid\n", ctx->label, cmd->gen.tag, newuid );
	ctx->real_driver->find_new_msgs( ctx->real_store, newuid, proxy_find_new_msgs_cb, cmd );
	debug( "%s[% 2d] Leave find_new_msgs\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, void *aux );
	void *callback_aux;
} set_msg_flags_cmd_t;

static void
proxy_set_msg_flags_cb( int sts, void *aux )
{
	set_msg_flags_cmd_t *cmd = (set_msg_flags_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter set_msg_flags, sts=%d\n", cmd->gen.ctx->label, cmd->gen.tag, sts );
	cmd->callback( sts, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave set_msg_flags\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_set_msg_flags( store_t *gctx, message_t *msg, uint uid, int add, int del, void (*cb)( int sts, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	set_msg_flags_cmd_t *cmd = (set_msg_flags_cmd_t *)proxy_cmd_new( ctx, sizeof(set_msg_flags_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	static char fbuf1[as(Flags) + 1], fbuf2[as(Flags) + 1];
	proxy_make_flags( add, fbuf1 );
	proxy_make_flags( del, fbuf2 );
	debug( "%s[% 2d] Enter set_msg_flags, uid=%u, add=%s, del=%s\n", ctx->label, cmd->gen.tag, uid, fbuf1, fbuf2 );
	ctx->real_driver->set_msg_flags( ctx->real_store, msg, uid, add, del, proxy_set_msg_flags_cb, cmd );
	debug( "%s[% 2d] Leave set_msg_flags\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, void *aux );
	void *callback_aux;
} trash_msg_cmd_t;

static void
proxy_trash_msg_cb( int sts, void *aux )
{
	trash_msg_cmd_t *cmd = (trash_msg_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter trash_msg, sts=%d\n", cmd->gen.ctx->label, cmd->gen.tag, sts );
	cmd->callback( sts, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave trash_msg\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_trash_msg( store_t *gctx, message_t *msg, void (*cb)( int sts, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	trash_msg_cmd_t *cmd = (trash_msg_cmd_t *)proxy_cmd_new( ctx, sizeof(trash_msg_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	debug( "%s[% 2d] Enter trash_msg, uid=%u\n", ctx->label, cmd->gen.tag, msg->uid );
	ctx->real_driver->trash_msg( ctx->real_store, msg, proxy_trash_msg_cb, cmd );
	debug( "%s[% 2d] Leave trash_msg\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( int sts, void *aux );
	void *callback_aux;
} close_box_cmd_t;

static void
proxy_close_box_cb( int sts, void *aux )
{
	close_box_cmd_t *cmd = (close_box_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter close_box, sts=%d\n", cmd->gen.ctx->label, cmd->gen.tag, sts );
	cmd->callback( sts, cmd->callback_aux );
	debug( "%s[% 2d] Callback leave close_box\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_close_box( store_t *gctx, void (*cb)( int sts, void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	close_box_cmd_t *cmd = (close_box_cmd_t *)proxy_cmd_new( ctx, sizeof(close_box_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	debug( "%s[% 2d] Enter close_box\n", ctx->label, cmd->gen.tag );
	ctx->real_driver->close_box( ctx->real_store, proxy_close_box_cb, cmd );
	debug( "%s[% 2d] Leave close_box\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

typedef struct {
	gen_cmd_t gen;
	void (*callback)( void *aux );
	void *callback_aux;
} cancel_cmds_cmd_t;

static void
proxy_cancel_cmds_cb( void *aux )
{
	cancel_cmds_cmd_t *cmd = (cancel_cmds_cmd_t *)aux;

	debug( "%s[% 2d] Callback enter cancel_cmds\n", cmd->gen.ctx->label, cmd->gen.tag );
	cmd->callback( cmd->callback_aux );
	debug( "%s[% 2d] Callback leave cancel_cmds\n", cmd->gen.ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static void
proxy_cancel_cmds( store_t *gctx, void (*cb)( void *aux ), void *aux )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	cancel_cmds_cmd_t *cmd = (cancel_cmds_cmd_t *)proxy_cmd_new( ctx, sizeof(cancel_cmds_cmd_t) );
	cmd->callback = cb;
	cmd->callback_aux = aux;
	debug( "%s[% 2d] Enter cancel_cmds\n", ctx->label, cmd->gen.tag );
	ctx->real_driver->cancel_cmds( ctx->real_store, proxy_cancel_cmds_cb, cmd );
	debug( "%s[% 2d] Leave cancel_cmds\n", ctx->label, cmd->gen.tag );
	proxy_cmd_done( &cmd->gen );
}

static int proxy_get_memory_usage( store_t *gctx )
{
	proxy_store_t *ctx = (proxy_store_t *)gctx;

	int rv = ctx->real_driver->get_memory_usage( ctx->real_store );
	debug( "%sCalled get_memory_usage, ret=%d\n", ctx->label, rv );
	return rv;
}

struct driver proxy_driver = {
	proxy_get_caps,
	0,
	0,
	0,
	proxy_set_bad_callback,
	proxy_connect_store,
	proxy_free_store,
	proxy_cancel_store,
	proxy_list_store,
	proxy_select_box,
	proxy_get_box_path,
	proxy_create_box,
	proxy_open_box,
	proxy_get_uidnext,
	proxy_confirm_box_empty,
	proxy_delete_box,
	proxy_finish_delete_box,
	proxy_prepare_load_box,
	proxy_load_box,
	proxy_fetch_msg,
	proxy_store_msg,
	proxy_find_new_msgs,
	proxy_set_msg_flags,
	proxy_trash_msg,
	proxy_close_box,
	proxy_cancel_cmds,
	proxy_commit_cmds,
	proxy_get_memory_usage,
	0,
};
