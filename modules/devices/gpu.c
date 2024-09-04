/*
 *    HardInfo - Displays System Information
 *    Copyright (C) 2003-2017 L. A. F. Pereira <l@tia.mat.br>
 *    This file
 *    Copyright (C) 2018 Burt P. <pburt0@gmail.com>
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

#include <string.h>

#include "hardinfo.h"
#include "devices.h"
#include "gpu_util.h"

gchar *gpu_list = NULL;
gchar *gpu_summary = NULL;

void gpu_summary_add(const char *gpu_name) {
    if (strlen(gpu_summary) == 0) {
        /* first one */
        gpu_summary = h_strdup_cprintf("%s", gpu_summary, gpu_name);
    } else {
        /* additional */
        gpu_summary = h_strdup_cprintf(" + %s", gpu_summary, gpu_name);
    }
}

#define UNKIFNULL_AC(f) (f != NULL) ? f : _("(Unknown)");

static void _gpu_pci_dev(gpud* gpu) {
    pcid *p = gpu->pci_dev;
    gchar *str;
    gchar *vendor, *svendor, *product, *sproduct;
    gchar *name, *key;
    gchar *drm_path = NULL;

    gboolean vendor_is_svendor = (p->vendor_id == p->sub_vendor_id && p->device_id == p->sub_device_id);

    vendor = UNKIFNULL_AC(p->vendor_id_str);
    svendor = UNKIFNULL_AC(p->sub_vendor_id_str);
    product = UNKIFNULL_AC(p->device_id_str);
    sproduct = UNKIFNULL_AC(p->sub_device_id_str);
    if (gpu->drm_dev)
        drm_path = g_strdup_printf("/dev/dri/%s", gpu->drm_dev);
    else
        drm_path = g_strdup(_("(Unknown)"));

    gchar *ven_tag = vendor_match_tag(p->vendor_id_str, params.fmt_opts);
    gchar *sven_tag = vendor_match_tag(p->sub_vendor_id_str, params.fmt_opts);
    if (ven_tag) {
        if (sven_tag && !vendor_is_svendor) {
            name = g_strdup_printf("%s %s %s", sven_tag, ven_tag, product);
        } else {
            name = g_strdup_printf("%s %s", ven_tag, product);
        }
    } else {
        name = g_strdup_printf("%s %s", vendor, product);
    }
    g_free(ven_tag);
    g_free(sven_tag);

    key = g_strdup_printf("GPU%s", gpu->id);

    gpu_summary_add((gpu->nice_name) ? gpu->nice_name : name);
    gpu_list = h_strdup_cprintf("$!%s$%s=%s\n", gpu_list, key, gpu->id, name);

    gchar *vendor_device_str;
    if (p->vendor_id == p->sub_vendor_id && p->device_id == p->sub_device_id) {
        vendor_device_str = g_strdup_printf(
                     /* Vendor */     "$^$%s=[%04x] %s\n"
                     /* Device */     "%s=[%04x] %s\n",
                    _("Vendor"), p->vendor_id, vendor,
                    _("Device"), p->device_id, product);
    } else {
        vendor_device_str = g_strdup_printf(
                     /* Vendor */     "$^$%s=[%04x] %s\n"
                     /* Device */     "%s=[%04x] %s\n"
                     /* Sub-device vendor */ "$^$%s=[%04x] %s\n"
                     /* Sub-device */     "%s=[%04x] %s\n",
                    _("Vendor"), p->vendor_id, vendor,
                    _("Device"), p->device_id, product,
                    _("SVendor"), p->sub_vendor_id, svendor,
                    _("SDevice"), p->sub_device_id, sproduct);
    }

    gchar *pcie_str;
    if (p->pcie_width_curr) {
        pcie_str = g_strdup_printf("[%s]\n"
                     /* Width (max) */  "%s=x%u\n"
                     /* Speed (max) */  "%s=%0.1f %s\n",
                    _("PCI Express"),
                    _("Maximum Link Width"), p->pcie_width_max,
                    _("Maximum Link Speed"), p->pcie_speed_max, _("GT/s") );
    } else
        pcie_str = strdup("");

    gchar *nv_str;
    if (gpu->nv_info) {
        nv_str = g_strdup_printf("[%s]\n"
                     /* model */  "%s=%s\n"
                     /* bios */   "%s=%s\n"
                     /* uuid */   "%s=%s\n",
                    _("NVIDIA"),
                    _("Model"), gpu->nv_info->model,
                    _("BIOS Version"), gpu->nv_info->bios_version,
                    _("UUID"), gpu->nv_info->uuid );
    } else
        nv_str = strdup("");

    gchar *freq = g_strdup(_("(Unknown)"));
    if (gpu->khz_max > 0) {
        if (gpu->khz_min > 0 && gpu->khz_min != gpu->khz_max)
            freq = g_strdup_printf("%0.2f-%0.2f %s", (double) gpu->khz_min / 1000, (double) gpu->khz_max / 1000, _("MHz"));
        else
            freq = g_strdup_printf("%0.2f %s", (double) gpu->khz_max / 1000, _("MHz"));
    }

    gchar *mem_freq = g_strdup(_("(Unknown)"));
    if (gpu->mem_khz_max > 0) {
        if (gpu->mem_khz_min > 0 && gpu->mem_khz_min != gpu->mem_khz_max)
            mem_freq = g_strdup_printf("%0.2f-%0.2f %s", (double) gpu->mem_khz_min / 1000, (double) gpu->mem_khz_max / 1000, _("MHz"));
        else
            mem_freq = g_strdup_printf("%0.2f %s", (double) gpu->mem_khz_max / 1000, _("MHz"));
    }

    //Add GPU name from renderer - this is only correct for 1 GPU systems, which are most builds
    gchar *t=NULL,*g=module_call_method("computer::getOGLRenderer");
    if(g) {
        int i=1;
	if( strstr(module_call_method("computer::getMachineType"),"irtual") ){//virtual
	    gpuname=g_strdup_printf("GPU=%s\n",module_call_method("computer::getMachineType"));
	} else if(strlen(g)>7 && g[0]=='l' && g[1]=='l' && g[2]=='v' && g[3]=='m' && g[4]=='p' && g[5]=='i' && g[6]=='p' && g[7]=='e'){
	    //Software - no hw accelleration drivers
	    if(strstr(vendor_device_str,"ntegrat")){//Integrated
	        gpuname=g_strdup_printf("GPU=Integrated (%s)\n",module_call_method("devices::getProcessorName"));
	    } else {//software render
	        gpuname=g_strdup_printf("GPU=Software (%s)\n",module_call_method("devices::getProcessorName"));
	    }
	} else if(strlen(g)>10 && g[0]=='D' && g[1]=='3' && g[2]=='D' && g[3]=='1' && g[4]=='2'){//WSL2
	    t=g+7;
            while(g[i]){
	        if((g[i]==')')) g[i]=0;
	        i++;
            }
            gpuname=g_strdup_printf("GPU=%s (D3D12)\n",t);
	} else if(strlen(g)>3 && g[0]=='N' && g[1]=='V' && g[2]!='i' && product && strstr(product,"[") && strstr(product,"]")){//nvidea with NVXXX
	    char *t=strstr(product,"[")+1;
	    char *tt=strstr(product,"]");*tt='\0';
	    gpuname=g_strdup_printf("GPU=NVidia %s (%s)\n",t,g);
	} else {//Graphics cards
            while(g[i]){
	        if((g[i-1]==' ') && (g[i]=='(')) g[i-1]=0;
	        i++;
            }
            gpuname=g_strdup_printf("GPU=%s\n",g);
	}
        g_free(g);
    }else gpuname=g_strdup("GPU=Unknown\n");//unknown as no renderer

    str = g_strdup_printf("[%s]\n"
	     /* GPU */	     "%s\n"
             /* Location */  "%s=%s\n"
             /* DRM Dev */   "%s=%s\n"
             /* Class */     "%s=[%04x] %s\n"
                             "%s"
             /* Revision */  "%s=%02x\n"
                             "[%s]\n"
             /* Core freq */ "%s=%s\n"
             /* Mem freq */  "%s=%s\n"
             /* NV */        "%s"
             /* PCIe */      "%s"
                             "[%s]\n"
             /* Driver */    "%s=%s\n"
             /* Modules */   "%s=%s\n",
                _("Device Information"),
		gpuname,
                _("Location"), gpu->location,
                _("DRM Device"), drm_path,
                _("Class"), p->class, p->class_str,
                vendor_device_str,
                _("Revision"), p->revision,
                _("Clocks"),
                _("Core"), freq,
                _("Memory"), mem_freq,
                nv_str,
                pcie_str,
                _("Driver"),
                _("In Use"), (p->driver) ? p->driver : _("(Unknown)"),
                _("Kernel Modules"), (p->driver_list) ? p->driver_list : _("(Unknown)")
                );

    moreinfo_add_with_prefix("DEV", key, str); /* str now owned by morinfo */

    g_free(drm_path);
    g_free(pcie_str);
    g_free(nv_str);
    g_free(vendor_device_str);
    g_free(name);
    g_free(key);
}

int _dt_soc_gpu(gpud *gpu) {
    static char UNKSOC[] = "(Unknown)"; /* don't translate this */
    gchar *vendor = gpu->vendor_str;
    gchar *device = gpu->device_str;
    if (vendor == NULL) vendor = UNKSOC;
    if (device == NULL) device = UNKSOC;
    gchar *freq = g_strdup(_("(Unknown)"));
    if (gpu->khz_max > 0) {
        if (gpu->khz_min > 0)
            freq = g_strdup_printf("%0.2f-%0.2f %s", (double) gpu->khz_min / 1000, (double) gpu->khz_max / 1000, _("MHz"));
        else
            freq = g_strdup_printf("%0.2f %s", (double) gpu->khz_max / 1000, _("MHz"));
    }
    gchar *key = g_strdup(gpu->id);

    gchar *name = NULL;
    gchar *vtag = vendor_match_tag(gpu->vendor_str, params.fmt_opts);
    if (vtag) {
        name = g_strdup_printf("%s %s", vtag, device);
    } else {
        name = (vendor == UNKSOC && device == UNKSOC)
            ? g_strdup(_("Unknown integrated GPU"))
            : g_strdup_printf("%s %s", vendor, device);
    }
    g_free(vtag);

    gchar *opp_str;
    if (gpu->dt_opp) {
        static const char *freq_src[] = {
            N_("clock-frequency property"),
            N_("Operating Points (OPPv1)"),
            N_("Operating Points (OPPv2)"),
        };
        opp_str = g_strdup_printf("[%s]\n"
                     /* MinFreq */  "%s=%d %s\n"
                     /* MaxFreq */  "%s=%d %s\n"
                     /* Latency */  "%s=%d %s\n"
                     /* Source */   "%s=%s\n",
                    _("Frequency Scaling"),
                    _("Minimum"), gpu->dt_opp->khz_min, _("kHz"),
                    _("Maximum"), gpu->dt_opp->khz_max, _("kHz"),
                    _("Transition Latency"), gpu->dt_opp->clock_latency_ns, _("ns"),
                    _("Source"), _(freq_src[gpu->dt_opp->version]) );
    } else
        opp_str = strdup("");

    gpu_summary_add((gpu->nice_name) ? gpu->nice_name : name);
    gpu_list = h_strdup_cprintf("$!%s$%s=%s\n", gpu_list, key, key, name);

    gpuname=g_strdup_printf("GPU=Integrated (%s)\n",module_call_method("devices::getProcessorName"));

    gchar *str = g_strdup_printf("[%s]\n"
             /* GPU */       "%s"
             /* Location */  "%s=%s\n"
             /* Vendor */  "$^$%s=%s\n"
             /* Device */  "%s=%s\n"
                           "[%s]\n"
             /* Freq */    "%s=%s\n"
             /* opp-v2 */  "%s"
                           "[%s]\n"
             /* Path */    "%s=%s\n"
             /* Compat */  "%s=%s\n"
             /* Status */  "%s=%s\n"
             /* Name */    "%s=%s\n",
                _("Device Information"),
		gpuname,
                _("Location"), gpu->location,
                _("Vendor"),   vendor,
                _("Device"),   device,
                _("Clocks"),
                _("Core"),     freq,
                opp_str,
                _("Device Tree Node"),
                _("Path"),     gpu->dt_path,
                _("Compatible"), gpu->dt_compat,
                _("Status"),   gpu->dt_status,
                _("Name"),     gpu->dt_name
                );
    moreinfo_add_with_prefix("DEV", key, str); /* str now owned by morinfo */
    g_free(freq);
    g_free(opp_str);
    return 1;
}

void scan_gpu_do(void) {
    if (gpu_summary)
        g_free(gpu_summary);
    if (gpu_list) {
        moreinfo_del_with_prefix("DEV:GPU");
        g_free(gpu_list);
    }
    gpu_summary = strdup("");
    gpu_list = g_strdup_printf("[%s]\n", _("GPUs"));

    gpud *gpus = gpu_get_device_list();
    gpud *curr = gpus;

    int c = gpud_list_count(gpus);

    if (c > 0) {
        while(curr) {
            if (curr->pci_dev) {
                _gpu_pci_dev(curr);
            }
            if (curr->dt_compat) {
                _dt_soc_gpu(curr);
            }
            curr=curr->next;
        }
    }
    gpud_list_free(gpus);

    if (c)
        gpu_list = g_strconcat(gpu_list, "[$ShellParam$]\n", "ViewType=1\n", NULL);
    else  {
        /* NO GPU? */
        gpu_list = g_strconcat(gpu_list, _("No GPU devices found"), "=\n", NULL);
    }
}
