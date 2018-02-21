#include "../routing.c"
#include <stdio.h>

struct broadcast_state *new_broadcast_state(tal_t *ctx UNNEEDED)
{
	return NULL;
}

void status_fmt(enum log_level level UNUSED, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vprintf(fmt, ap);
	printf("\n");
	va_end(ap);
}

/* AUTOGENERATED MOCKS START */
/* Generated stub for fromwire_channel_announcement */
bool fromwire_channel_announcement(const tal_t *ctx UNNEEDED, const void *p UNNEEDED, secp256k1_ecdsa_signature *node_signature_1 UNNEEDED, secp256k1_ecdsa_signature *node_signature_2 UNNEEDED, secp256k1_ecdsa_signature *bitcoin_signature_1 UNNEEDED, secp256k1_ecdsa_signature *bitcoin_signature_2 UNNEEDED, u8 **features UNNEEDED, struct bitcoin_blkid *chain_hash UNNEEDED, struct short_channel_id *short_channel_id UNNEEDED, struct pubkey *node_id_1 UNNEEDED, struct pubkey *node_id_2 UNNEEDED, struct pubkey *bitcoin_key_1 UNNEEDED, struct pubkey *bitcoin_key_2 UNNEEDED)
{ fprintf(stderr, "fromwire_channel_announcement called!\n"); abort(); }
/* Generated stub for fromwire_channel_update */
bool fromwire_channel_update(const void *p UNNEEDED, secp256k1_ecdsa_signature *signature UNNEEDED, struct bitcoin_blkid *chain_hash UNNEEDED, struct short_channel_id *short_channel_id UNNEEDED, u32 *timestamp UNNEEDED, u16 *flags UNNEEDED, u16 *cltv_expiry_delta UNNEEDED, u64 *htlc_minimum_msat UNNEEDED, u32 *fee_base_msat UNNEEDED, u32 *fee_proportional_millionths UNNEEDED)
{ fprintf(stderr, "fromwire_channel_update called!\n"); abort(); }
/* Generated stub for fromwire_node_announcement */
bool fromwire_node_announcement(const tal_t *ctx UNNEEDED, const void *p UNNEEDED, secp256k1_ecdsa_signature *signature UNNEEDED, u8 **features UNNEEDED, u32 *timestamp UNNEEDED, struct pubkey *node_id UNNEEDED, u8 rgb_color[3] UNNEEDED, u8 alias[32] UNNEEDED, u8 **addresses UNNEEDED)
{ fprintf(stderr, "fromwire_node_announcement called!\n"); abort(); }
/* Generated stub for fromwire_peektype */
int fromwire_peektype(const u8 *cursor UNNEEDED)
{ fprintf(stderr, "fromwire_peektype called!\n"); abort(); }
/* Generated stub for fromwire_u8 */
u8 fromwire_u8(const u8 **cursor UNNEEDED, size_t *max UNNEEDED)
{ fprintf(stderr, "fromwire_u8 called!\n"); abort(); }
/* Generated stub for fromwire_wireaddr */
bool fromwire_wireaddr(const u8 **cursor UNNEEDED, size_t *max UNNEEDED, struct wireaddr *addr UNNEEDED)
{ fprintf(stderr, "fromwire_wireaddr called!\n"); abort(); }
/* Generated stub for onion_type_name */
const char *onion_type_name(int e UNNEEDED)
{ fprintf(stderr, "onion_type_name called!\n"); abort(); }
/* Generated stub for replace_broadcast */
bool replace_broadcast(struct broadcast_state *bstate UNNEEDED,
		       u64 *index UNNEEDED,
		       const int type UNNEEDED,
		       const u8 *tag UNNEEDED,
		       const u8 *payload UNNEEDED)
{ fprintf(stderr, "replace_broadcast called!\n"); abort(); }
/* Generated stub for status_failed */
void status_failed(enum status_failreason code UNNEEDED,
		   const char *fmt UNNEEDED, ...)
{ fprintf(stderr, "status_failed called!\n"); abort(); }
/* Generated stub for towire_pubkey */
void towire_pubkey(u8 **pptr UNNEEDED, const struct pubkey *pubkey UNNEEDED)
{ fprintf(stderr, "towire_pubkey called!\n"); abort(); }
/* Generated stub for towire_short_channel_id */
void towire_short_channel_id(u8 **pptr UNNEEDED,
			     const struct short_channel_id *short_channel_id UNNEEDED)
{ fprintf(stderr, "towire_short_channel_id called!\n"); abort(); }
/* Generated stub for towire_u16 */
void towire_u16(u8 **pptr UNNEEDED, u16 v UNNEEDED)
{ fprintf(stderr, "towire_u16 called!\n"); abort(); }
/* AUTOGENERATED MOCKS END */

const void *trc;

/* Updates existing route if required. */
static struct node_connection *add_connection(struct routing_state *rstate,
					      const struct pubkey *from,
					      const struct pubkey *to,
					      u32 base_fee, s32 proportional_fee,
					      u32 delay)
{
	struct node_connection *c = get_or_make_connection(rstate, from, to);
	c->base_fee = base_fee;
	c->proportional_fee = proportional_fee;
	c->delay = delay;
	c->active = true;
	memset(&c->short_channel_id, 0, sizeof(c->short_channel_id));
	c->flags = get_channel_direction(from, to);
	return c;
}

int main(void)
{
	static const struct bitcoin_blkid zerohash;
	const tal_t *ctx = trc = tal_tmpctx(NULL);
	struct node_connection *nc;
	struct routing_state *rstate;
	struct pubkey a, b, c, d;
	struct privkey tmp;
	u64 fee;
	struct node_connection **route;
	const double riskfactor = 1.0 / BLOCKS_PER_YEAR / 10000;

	secp256k1_ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY
						 | SECP256K1_CONTEXT_SIGN);

	memset(&tmp, 'a', sizeof(tmp));
	rstate = new_routing_state(ctx, &zerohash, &a);

	pubkey_from_privkey(&tmp, &a);
	new_node(rstate, &a);

	memset(&tmp, 'b', sizeof(tmp));
	pubkey_from_privkey(&tmp, &b);
	new_node(rstate, &b);

	/* A<->B */
	add_connection(rstate, &a, &b, 1, 1, 1);

	nc = find_route(ctx, rstate, &a, &b, 1000, riskfactor, &fee, &route);
	assert(nc);
	assert(tal_count(route) == 0);
	assert(fee == 0);

	/* A<->B<->C */
	memset(&tmp, 'c', sizeof(tmp));
	pubkey_from_privkey(&tmp, &c);
	new_node(rstate, &c);

	status_trace("A = %s", type_to_string(trc, struct pubkey, &a));
	status_trace("B = %s", type_to_string(trc, struct pubkey, &b));
	status_trace("C = %s", type_to_string(trc, struct pubkey, &c));
	add_connection(rstate, &b, &c, 1, 1, 1);

	nc = find_route(ctx, rstate, &a, &c, 1000, riskfactor, &fee, &route);
	assert(nc);
	assert(tal_count(route) == 1);
	assert(fee == 1);

	/* A<->D<->C: Lower base, higher percentage. */
	memset(&tmp, 'd', sizeof(tmp));
	pubkey_from_privkey(&tmp, &d);
	new_node(rstate, &d);
	status_trace("D = %s", type_to_string(trc, struct pubkey, &d));

	add_connection(rstate, &a, &d, 0, 2, 1);
	add_connection(rstate, &d, &c, 0, 2, 1);

	/* Will go via D for small amounts. */
	nc = find_route(ctx, rstate, &a, &c, 1000, riskfactor, &fee, &route);
	assert(nc);
	assert(tal_count(route) == 1);
	assert(pubkey_eq(&route[0]->src->id, &d));
	assert(fee == 0);

	/* Will go via B for large amounts. */
	nc = find_route(ctx, rstate, &a, &c, 3000000, riskfactor, &fee, &route);
	assert(nc);
	assert(tal_count(route) == 1);
	assert(pubkey_eq(&route[0]->src->id, &b));
	assert(fee == 1 + 3);

	/* Make B->C inactive, force it back via D */
	get_connection(rstate, &b, &c)->active = false;
	nc = find_route(ctx, rstate, &a, &c, 3000000, riskfactor, &fee, &route);
	assert(nc);
	assert(tal_count(route) == 1);
	assert(pubkey_eq(&route[0]->src->id, &d));
	assert(fee == 0 + 6);

	tal_free(ctx);
	secp256k1_context_destroy(secp256k1_ctx);
	return 0;
}
