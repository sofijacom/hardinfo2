/*
 *    HardInfo - Displays System Information
 *    Copyright (C) 2003-2017 L. A. F. Pereira <l@tia.mat.br>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, version 2 or later.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include "hardinfo.h"
#include "benchmark.h"
#include "blowfish.h"

/* if anything changes in this block, increment revision */
#define BENCH_REVISION 3
#define CRUNCH_TIME 7
#define BENCH_DATA_SIZE 65536
#define BENCH_DATA_MD5 "c25cf5c889f7bead2ff39788eedae37b"
#define BLOW_KEY "Has my shampoo arrived?"
#define BLOW_KEY_MD5 "6eac709cca51a228bfa70150c9c5a7c4"

static gpointer bfish_exec(const void *in_data, gint thread_number)
{
    char key[] = BLOW_KEY;
    unsigned char *data = NULL;
    unsigned long data_len = BENCH_DATA_SIZE, i = 0;
    BLOWFISH_CTX ctx;

    data = g_malloc(BENCH_DATA_SIZE);
    if(data){
        memcpy(data, in_data, BENCH_DATA_SIZE);
        Blowfish_Init(&ctx, (guchar *)key, strlen(key));
        for(i = 0; i < (data_len-sizeof(guint32)); i += 2*sizeof(guint32)) {
	    Blowfish_Encrypt(&ctx, (guint32 *)&data[i], (guint32 *)&data[i+sizeof(guint32)]);
        }
        for(i = 0; i < (data_len-sizeof(guint32)); i += 2*sizeof(guint32)) {
            Blowfish_Decrypt(&ctx, (guint32 *)&data[i], (guint32 *)&data[i+sizeof(guint32)]);
        }
        g_free(data);
    }
    return NULL;
}

void benchmark_bfish_do(int threads, int entry, const char *status)
{
    bench_value r = EMPTY_BENCH_VALUE;
    gchar *test_data = get_test_data(BENCH_DATA_SIZE);
    if (!test_data) return;

    shell_view_set_enabled(FALSE);
    shell_status_update(status);

    gchar *k = md5_digest_str(BLOW_KEY, strlen(BLOW_KEY));
    if (!SEQ(k, BLOW_KEY_MD5))
        bench_msg("test key has different md5sum: expected %s, actual %s", BLOW_KEY_MD5, k);
    gchar *d = md5_digest_str(test_data, BENCH_DATA_SIZE);
    if (!SEQ(d, BENCH_DATA_MD5))
        bench_msg("test data has different md5sum: expected %s, actual %s", BENCH_DATA_MD5, d);

    r = benchmark_crunch_for(CRUNCH_TIME, threads, bfish_exec, test_data);
    r.result /= 100;
    r.revision = BENCH_REVISION;
    snprintf(r.extra, 255, "%0.1fs, k:%s, d:%s", (double)CRUNCH_TIME, k, d);

    g_free(test_data);
    g_free(k);
    g_free(d);

    bench_results[entry] = r;
}

void benchmark_bfish_threads(void) { benchmark_bfish_do(0, BENCHMARK_BLOWFISH_THREADS, "Performing Blowfish benchmark (multi-thread)..."); }
void benchmark_bfish_single(void) { benchmark_bfish_do(1, BENCHMARK_BLOWFISH_SINGLE, "Performing Blowfish benchmark (single-thread)..."); }
void benchmark_bfish_cores(void) { benchmark_bfish_do(-1, BENCHMARK_BLOWFISH_CORES, "Performing Blowfish benchmark (multi-core)..."); }
