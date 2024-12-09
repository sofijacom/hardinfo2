/*
 * spd-decode.h including SPD vendor database
 * Copyright (c) 2024 hwspeedy
 *
 * Based on decode-dimms.pl
 * Copyright 1998, 1999 Philip Edelbrock <phil@netroedge.com>
 * modified by Christian Zuckschwerdt <zany@triq.net>
 * modified by Burkart Lingner <burkart@bollchen.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define UNKIFNULL2(f) ((f) ? f : _("(Unknown)"))
#define UNKIFEMPTY2(f) ((*f) ? f : _("(Unknown)"))
#define STR_IGNORE(str, ignore) if (SEQ(str, ignore)) { *str = 0; null_if_empty(&str); }
#define GET_RAM_TYPE_STR(rt) (ram_types[(rt < N_RAM_TYPES) ? rt : 0])
#define UNKNOWN_MEM_TYPE_STRING _("RAM")

typedef enum {
    UNKNOWN           = 0,
    DIRECT_RAMBUS     = 1,
    RAMBUS            = 2,
    FPM_DRAM          = 3,
    EDO               = 4,
    PIPELINED_NIBBLE  = 5,
    SDR_SDRAM         = 6,
    MULTIPLEXED_ROM   = 7,
    DDR_SGRAM         = 8,
    DDR_SDRAM         = 9,
    DDR2_SDRAM        = 10,
    DDR3_SDRAM        = 11,
    DDR4_SDRAM        = 12,
    DDR5_SDRAM        = 13,
    N_RAM_TYPES       = 14
} RamType;

static const char *ram_types[] = {
    "Unknown",
    "Direct Rambus",
    "Rambus",
    "FPM DRAM",
    "EDO",
    "Pipelined Nibble",
    "SDR SDRAM",
    "Multiplexed ROM",
    "DDR SGRAM",
    "DDR SDRAM",
    "DDR2 SDRAM",
    "DDR3 SDRAM",
    "DDR4 SDRAM",
    "DDR5 SDRAM"
};

typedef uint32_t dmi_mem_size;
struct dmi_mem_socket;
typedef struct {
    unsigned char *bytes;//allocated
    char dev[32];
    int spd_size;
    RamType type;
    int vendor_bank;
    int vendor_index;
    int dram_vendor_bank;
    int dram_vendor_index;
    char partno[32];
    char serialno[32];
    const char *form_factor;
    char type_detail[256];
    dmi_mem_size size_MiB;
    int spd_rev_major;
    int spd_rev_minor;
    int week, year;
    gboolean ddr4_no_ee1004;
    int match_score;
    const char *spd_driver;//link to static const
    char *vendor_str;//links to static const
    char *dram_vendor_str;//links to static const
    struct dmi_mem_socket *dmi_socket;//links
    const Vendor *vendor;//links
    const Vendor *dram_vendor;//links
} spd_data;

GSList *spd_scan();
gchar *make_spd_section(spd_data *spd);
void spd_data_free(spd_data *s);



/* from decode-dimms, in the i2c-tools package:
 * https://mirrors.edge.kernel.org/pub/software/utils/i2c-tools/
 *
 * STANDARD MANUFACTURERS IDENTIFICATION CODEs
 * as defined in JEP106
 *
 * As of: 13 August 2024
 */

#define VENDORS_BANKS 13
#define VENDORS_ITEMS 128
#define JEDEC_MFG_STR(b,i) ( (b >= 0 && b < VENDORS_BANKS && i < VENDORS_ITEMS) ? vendors[(b)][(i)] : NULL )
static const char* vendors[VENDORS_BANKS][VENDORS_ITEMS] =
{

{"AMD", "AMI", "Fairchild", "Fujitsu",
 "GTE", "Harris", "Hitachi", "Inmos",
 "Intel", "I.T.T.", "Intersil", "Monolithic Memories",
 "Mostek", "Freescale (former Motorola)", "National", "NEC",
 "RCA", "Raytheon", "Conexant (Rockwell)", "Seeq",
 "NXP (former Signetics, Philips Semi.)", "Synertek", "Texas Instruments", "Kioxia Corporation (former Toshiba Memory Corporation)",
 "Xicor", "Zilog", "Eurotechnique", "Mitsubishi",
 "Lucent (AT&T)", "Exel", "Atmel", "STMicroelectronics (former SGS/Thomson)",
 "Lattice Semi.", "NCR", "Wafer Scale Integration", "IBM",
 "Tristar", "Visic", "Intl. CMOS Technology", "SSSI",
 "MicrochipTechnology", "Ricoh Ltd.", "VLSI", "Micron Technology",
 "SK Hynix (former Hyundai Electronics)", "OKI Semiconductor", "ACTEL", "Sharp",
 "Catalyst", "Panasonic", "IDT", "Cypress",
 "DEC", "LSI Logic", "Zarlink (former Plessey)", "UTMC",
 "Thinking Machine", "Thomson CSF", "Integrated CMOS (Vertex)", "Honeywell",
 "Tektronix", "Oracle Corporation (former Sun Microsystems)", "Silicon Storage Technology", "ProMos/Mosel Vitelic",
 "Infineon (former Siemens)", "Macronix", "Xerox", "Plus Logic",
 "Western Digital Technologies (former SanDisk Corporation)", "Elan Circuit Tech.", "European Silicon Str.", "Apple Computer",
 "Xilinx", "Compaq", "Protocol Engines", "SCI",
 "Seiko Instruments", "Samsung", "I3 Design System", "Klic",
 "Crosspoint Solutions", "Alliance Memory Inc", "Tandem", "Hewlett-Packard",
 "Integrated Silicon Solutions", "Brooktree", "New Media", "MHS Electronic",
 "Performance Semi.", "Winbond Electronic", "Kawasaki Steel", "Bright Micro",
 "TECMAR", "Exar", "PCMCIA", "LG Semi (former Goldstar)",
 "Northern Telecom", "Sanyo", "Array Microsystems", "Crystal Semiconductor",
 "Analog Devices", "PMC-Sierra", "Asparix", "Convex Computer",
 "Quality Semiconductor", "Nimbus Technology", "Transwitch", "Micronas (ITT Intermetall)",
 "Cannon", "Altera", "NEXCOM", "QUALCOMM",
 "Sony", "Cray Research", "AMS(Austria Micro)", "Vitesse",
 "Aster Electronics", "Bay Networks (Synoptic)", "Zentrum or ZMD", "TRW",
 "Thesys", "Solbourne Computer", "Allied-Signal", "Dialog",
 "Media Vision", "Numonyx Corporation (former Level One Communication)"},
{"Cirrus Logic", "National Instruments", "ILC Data Device", "Alcatel Mietec",
 "Micro Linear", "Univ. of NC", "JTAG Technologies", "BAE Systems",
 "Nchip", "Galileo Tech", "Bestlink Systems", "Graychip",
 "GENNUM", "VideoLogic", "Robert Bosch", "Chip Express",
 "DATARAM", "United Microelec Corp.", "TCSI", "Smart Modular",
 "Hughes Aircraft", "Lanstar Semiconductor", "Qlogic", "Kingston",
 "Music Semi", "Ericsson Components", "SpaSE", "Eon Silicon Devices",
 "Integrated Silicon Solution (ISSI) (former Programmable Micro Corp)", "DoD", "Integ. Memories Tech.", "Corollary Inc.",
 "Dallas Semiconductor", "Omnivision", "EIV(Switzerland)", "Novatel Wireless",
 "Zarlink (former Mitel)", "Clearpoint", "Cabletron", "STEC (former Silicon Technology)",
 "Vanguard", "Hagiwara Sys-Com", "Vantis", "Celestica",
 "Century", "Hal Computers", "Rohm Company Ltd.", "Juniper Networks",
 "Libit Signal Processing", "Mushkin Enhanced Memory", "Tundra Semiconductor", "Adaptec Inc.",
 "LightSpeed Semi.", "ZSP Corp.", "AMIC Technology", "Adobe Systems",
 "Dynachip", "PNY Technologies Inc. (former PNY Electronics)", "Newport Digital", "MMC Networks",
 "T Square", "Seiko Epson", "Broadcom", "Viking Components",
 "V3 Semiconductor", "Flextronics (former Orbit)", "Suwa Electronics", "Transmeta",
 "Micron CMS", "American Computer & Digital Components Inc", "Enhance 3000 Inc", "Tower Semiconductor",
 "CPU Design", "Price Point", "Maxim Integrated Product", "Tellabs",
 "Centaur Technology", "Unigen Corporation", "Transcend Information", "Memory Card Technology",
 "CKD Corporation Ltd.", "Capital Instruments, Inc.", "Aica Kogyo, Ltd.", "Linvex Technology",
 "MSC Vertriebs GmbH", "AKM Company, Ltd.", "Dynamem, Inc.", "NERA ASA",
 "GSI Technology", "Dane-Elec (C Memory)", "Acorn Computers", "Lara Technology",
 "Oak Technology, Inc.", "Itec Memory", "Tanisys Technology", "Truevision",
 "Wintec Industries", "Super PC Memory", "MGV Memory", "Galvantech",
 "Gadzoox Nteworks", "Multi Dimensional Cons.", "GateField", "Integrated Memory System",
 "Triscend", "XaQti", "Goldenram", "Clear Logic",
 "Cimaron Communications", "Nippon Steel Semi. Corp.", "Advantage Memory", "AMCC",
 "LeCroy", "Yamaha Corporation", "Digital Microwave", "NetLogic Microsystems",
 "MIMOS Semiconductor", "Advanced Fibre", "BF Goodrich Data.", "Epigram",
 "Acbel Polytech Inc.", "Apacer Technology", "Admor Memory", "FOXCONN",
 "Quadratics Superconductor", "3COM"},
{"Camintonn Corporation", "ISOA Incorporated", "Agate Semiconductor", "ADMtek Incorporated",
 "HYPERTEC", "Adhoc Technologies", "MOSAID Technologies", "Ardent Technologies",
 "Switchcore", "Cisco Systems, Inc.", "Allayer Technologies", "WorkX AG (Wichman)",
 "Oasis Semiconductor", "Novanet Semiconductor", "E-M Solutions", "Power General",
 "Advanced Hardware Arch.", "Inova Semiconductors GmbH", "Telocity", "Delkin Devices",
 "Symagery Microsystems", "C-Port Corporation", "SiberCore Technologies", "Southland Microsystems",
 "Malleable Technologies", "Kendin Communications", "Great Technology Microcomputer", "Sanmina Corporation",
 "HADCO Corporation", "Corsair", "Actrans System Inc.", "ALPHA Technologies",
 "Silicon Laboratories, Inc. (Cygnal)", "Artesyn Technologies", "Align Manufacturing", "Peregrine Semiconductor",
 "Chameleon Systems", "Aplus Flash Technology", "MIPS Technologies", "Chrysalis ITS",
 "ADTEC Corporation", "Kentron Technologies", "Win Technologies", "Tezzaron Semiconductor (former Tachyon Semiconductor)",
 "Extreme Packet Devices", "RF Micro Devices", "Siemens AG", "Sarnoff Corporation",
 "Itautec SA (former Itautec Philco SA)", "Radiata Inc.", "Benchmark Elect. (AVEX)", "Legend",
 "SpecTek Incorporated", "Hi/fn", "Enikia Incorporated", "SwitchOn Networks",
 "AANetcom Incorporated", "Micro Memory Bank", "ESS Technology", "Virata Corporation",
 "Excess Bandwidth", "West Bay Semiconductor", "DSP Group", "Newport Communications",
 "Chip2Chip Incorporated", "Phobos Corporation", "Intellitech Corporation", "Nordic VLSI ASA",
 "Ishoni Networks", "Silicon Spice", "Alchemy Semiconductor", "Agilent Technologies",
 "Centillium Communications", "W.L. Gore", "HanBit Electronics", "GlobeSpan",
 "Element 14", "Pycon", "Saifun Semiconductors", "Sibyte, Incorporated",
 "MetaLink Technologies", "Feiya Technology", "I & C Technology", "Shikatronics",
 "Elektrobit", "Megic", "Com-Tier", "Malaysia Micro Solutions",
 "Hyperchip", "Gemstone Communications", "Anadigm (former Anadyne)", "3ParData",
 "Mellanox Technologies", "Tenx Technologies", "Helix AG", "Domosys",
 "Skyup Technology", "HiNT Corporation", "Chiaro", "MDT Technologies GmbH (former MCI Computer GMBH)",
 "Exbit Technology A/S", "Integrated Technology Express", "AVED Memory", "Legerity",
 "Jasmine Networks", "Caspian Networks", "nCUBE", "Silicon Access Networks",
 "FDK Corporation", "High Bandwidth Access", "MultiLink Technology", "BRECIS",
 "World Wide Packets", "APW", "Chicory Systems", "Xstream Logic",
 "Fast-Chip", "Zucotto Wireless", "Realchip", "Galaxy Power",
 "eSilicon", "Morphics Technology", "Accelerant Networks", "Silicon Wave",
 "SandCraft", "Elpida"},
{"Solectron", "Optosys Technologies", "Buffalo (former Melco)", "TriMedia Technologies",
 "Cyan Technologies", "Global Locate", "Optillion", "Terago Communications",
 "Ikanos Communications", "Princeton Technology", "Nanya Technology", "Elite Flash Storage",
 "Mysticom", "LightSand Communications", "ATI Technologies", "Agere Systems",
 "NeoMagic", "AuroraNetics", "Golden Empire", "Mushkin",
 "Tioga Technologies", "Netlist", "TeraLogic", "Cicada Semiconductor",
 "Centon Electronics", "Tyco Electronics", "Magis Works", "Zettacom",
 "Cogency Semiconductor", "Chipcon AS", "Aspex Technology", "F5 Networks",
 "Programmable Silicon Solutions", "ChipWrights", "Acorn Networks", "Quicklogic",
 "Kingmax Semiconductor", "BOPS", "Flasys", "BitBlitz Communications",
 "eMemory Technology", "Procket Networks", "Purple Ray", "Trebia Networks",
 "Delta Electronics", "Onex Communications", "Ample Communications", "Memory Experts Intl",
 "Astute Networks", "Azanda Network Devices", "Dibcom", "Tekmos",
 "API NetWorks", "Bay Microsystems", "Firecron Ltd", "Resonext Communications",
 "Tachys Technologies", "Equator Technology", "Concept Computer", "SILCOM",
 "3Dlabs", "c't Magazine", "Sanera Systems", "Silicon Packets",
 "Viasystems Group", "Simtek", "Semicon Devices Singapore", "Satron Handelsges",
 "Improv Systems", "INDUSYS GmbH", "Corrent", "Infrant Technologies",
 "Ritek Corp", "empowerTel Networks", "Hypertec", "Cavium Networks",
 "PLX Technology", "Massana Design", "Intrinsity", "Valence Semiconductor",
 "Terawave Communications", "IceFyre Semiconductor", "Primarion", "Picochip Designs Ltd",
 "Silverback Systems", "Jade Star Technologies", "Pijnenburg Securealink",
 "takeMS - Ultron AG (former Memorysolution GmbH)", "Cambridge Silicon Radio",
 "Swissbit", "Nazomi Communications", "eWave System",
 "Rockwell Collins", "Picocel Co., Ltd.", "Alphamosaic Ltd", "Sandburst",
 "SiCon Video", "NanoAmp Solutions", "Ericsson Technology", "PrairieComm",
 "Mitac International", "Layer N Networks", "MtekVision", "Allegro Networks",
 "Marvell Semiconductors", "Netergy Microelectronic", "NVIDIA", "Internet Machines",
 "Memorysolution GmbH (former Peak Electronics)", "Litchfield Communication", "Accton Technology", "Teradiant Networks",
 "Scaleo Chip (former Europe Technologies)", "Cortina Systems", "RAM Components", "Raqia Networks",
 "ClearSpeed", "Matsushita Battery", "Xelerated", "SimpleTech",
 "Utron Technology", "Astec International", "AVM gmbH", "Redux Communications",
 "Dot Hill Systems", "TeraChip"},
{"T-RAM Incorporated", "Innovics Wireless", "Teknovus", "KeyEye Communications",
 "Runcom Technologies", "RedSwitch", "Dotcast", "Silicon Mountain Memory",
 "Signia Technologies", "Pixim", "Galazar Networks", "White Electronic Designs",
 "Patriot Scientific", "Neoaxiom Corporation", "3Y Power Technology", "Scaleo Chip (former Europe Technologies)",
 "Potentia Power Systems", "C-guys Incorporated", "Digital Communications Technology Incorporated", "Silicon-Based Technology",
 "Fulcrum Microsystems", "Positivo Informatica Ltd", "XIOtech Corporation", "PortalPlayer",
 "Zhiying Software", "Parker Vision, Inc. (former Direct2Data)", "Phonex Broadband", "Skyworks Solutions",
 "Entropic Communications", "I'M Intelligent Memory Ltd (former Pacific Force Technology)", "Zensys A/S", "Legend Silicon Corp.",
 "sci-worx GmbH", "SMSC (former Oasis Silicon Systems)", "Renesas Electronics (former Renesas Technology)", "Raza Microelectronics",
 "Phyworks", "MediaTek", "Non-cents Productions", "US Modular",
 "Wintegra Ltd", "Mathstar", "StarCore", "Oplus Technologies",
 "Mindspeed", "Just Young Computer", "Radia Communications", "OCZ",
 "Emuzed", "LOGIC Devices", "Inphi Corporation", "Quake Technologies",
 "Vixel", "SolusTek", "Kongsberg Maritime", "Faraday Technology",
 "Altium Ltd.", "Insyte", "ARM Ltd.", "DigiVision",
 "Vativ Technologies", "Endicott Interconnect Technologies", "Pericom", "Bandspeed",
 "LeWiz Communications", "CPU Technology", "Ramaxel Technology", "DSP Group",
 "Axis Communications", "Legacy Electronics", "Chrontel", "Powerchip Semiconductor",
 "MobilEye Technologies", "Excel Semiconductor", "A-DATA Technology", "VirtualDigm",
 "G Skill Intl", "Quanta Computer", "Yield Microelectronics", "Afa Technologies",
 "KINGBOX Technology Co. Ltd.", "Ceva", "iStor Networks", "Advance Modules",
 "Microsoft", "Open-Silicon", "Goal Semiconductor", "ARC International",
 "Simmtec", "Metanoia", "Key Stream", "Lowrance Electronics",
 "Adimos", "SiGe Semiconductor", "Fodus Communications", "Credence Systems Corp.",
 "Genesis Microchip Inc.", "Vihana, Inc.", "WIS Technologies", "GateChange Technologies",
 "High Density Devices AS", "Synopsys", "Gigaram", "Enigma Semiconductor Inc.",
 "Century Micro Inc.", "Icera Semiconductor", "Mediaworks Integrated Systems", "O'Neil Product Development",
 "Supreme Top Technology Ltd.", "MicroDisplay Corporation", "Team Group Inc.", "Sinett Corporation",
 "Toshiba Corporation", "Tensilica", "SiRF Technology", "Bacoc Inc.",
 "SMaL Camera Technologies", "Thomson SC", "Airgo Networks", "Wisair Ltd.",
 "SigmaTel", "Arkados", "Compete IT gmbH Co. KG", "Eudar Technology Inc.",
 "Focus Enhancements", "Xyratex"},
{"Specular Networks", "Patriot Memory", "U-Chip Technology Corp.", "Silicon Optix",
 "Greenfield Networks", "CompuRAM GmbH", "Stargen, Inc.", "NetCell Corporation",
 "Excalibrus Technologies Ltd", "SCM Microsystems", "Xsigo Systems, Inc.", "CHIPS & Systems Inc",
 "Tier 1 Multichip Solutions", "CWRL Labs", "Teradici", "Gigaram, Inc.",
 "g2 Microsystems", "PowerFlash Semiconductor", "P.A. Semi, Inc.", "NovaTech Solutions, S.A.",
 "c2 Microsystems, Inc.", "Level5 Networks", "COS Memory AG", "Innovasic Semiconductor",
 "02IC Co. Ltd", "Tabula, Inc.", "Crucial Technology", "Chelsio Communications",
 "Solarflare Communications", "Xambala Inc.", "EADS Astrium", "Terra Semiconductor Inc. (former ATO Semicon Co. Ltd.)",
 "Imaging Works, Inc.", "Astute Networks, Inc.", "Tzero", "Emulex",
 "Power-One", "Pulse~LINK Inc.", "Hon Hai Precision Industry", "White Rock Networks Inc.",
 "Telegent Systems USA, Inc.", "Atrua Technologies, Inc.", "Acbel Polytech Inc.",
 "eRide Inc.","ULi Electronics Inc.", "Magnum Semiconductor Inc.", "neoOne Technology, Inc.",
 "Connex Technology, Inc.", "Stream Processors, Inc.", "Focus Enhancements", "Telecis Wireless, Inc.",
 "uNav Microelectronics", "Tarari, Inc.", "Ambric, Inc.", "Newport Media, Inc.", "VMTS",
 "Enuclia Semiconductor, Inc.", "Virtium Technology Inc.", "Solid State System Co., Ltd.", "Kian Tech LLC",
 "Artimi", "Power Quotient International", "Avago Technologies", "ADTechnology", "Sigma Designs",
 "SiCortex, Inc.", "Ventura Technology Group", "eASIC", "M.H.S. SAS", "Micro Star International",
 "Rapport Inc.", "Makway International", "Broad Reach Engineering Co.",
 "Semiconductor Mfg Intl Corp", "SiConnect", "FCI USA Inc.", "Validity Sensors",
 "Coney Technology Co. Ltd.", "Spans Logic", "Neterion Inc.", "Qimonda",
 "New Japan Radio Co. Ltd.", "Velogix", "Montalvo Systems", "iVivity Inc.", "Walton Chaintech",
 "AENEON", "Lorom Industrial Co. Ltd.", "Radiospire Networks", "Sensio Technologies, Inc.",
 "Nethra Imaging", "Hexon Technology Pte Ltd", "CompuStocx (CSX)", "Methode Electronics, Inc.",
 "Connect One Ltd.", "Opulan Technologies", "Septentrio NV", "Goldenmars Technology Inc.",
 "Kreton Corporation", "Cochlear Ltd.", "Altair Semiconductor", "NetEffect, Inc.",
 "Spansion, Inc.", "Taiwan Semiconductor Mfg", "Emphany Systems Inc.",
 "ApaceWave Technologies", "Mobilygen Corporation", "Tego", "Cswitch Corporation",
 "Haier (Beijing) IC Design Co.", "MetaRAM", "Axel Electronics Co. Ltd.", "Tilera Corporation",
 "Aquantia", "Vivace Semiconductor", "Redpine Signals", "Octalica", "InterDigital Communications",
 "Avant Technology", "Asrock, Inc.", "Availink", "Quartics, Inc.", "Element CXI",
 "Innovaciones Microelectronicas", "VeriSilicon Microelectronics", "W5 Networks"},
{"MOVEKING", "Mavrix Technology, Inc.", "CellGuide Ltd.", "Faraday Technology",
 "Diablo Technologies, Inc.", "Jennic", "Octasic", "Molex Incorporated", "3Leaf Networks",
 "Bright Micron Technology", "Netxen", "NextWave Broadband Inc.", "DisplayLink", "ZMOS Technology",
 "Tec-Hill", "Multigig, Inc.", "Amimon", "Euphonic Technologies, Inc.", "BRN Phoenix",
 "InSilica", "Ember Corporation", "Avexir Technologies Corporation", "Echelon Corporation",
 "Edgewater Computer Systems", "XMOS Semiconductor Ltd.", "GENUSION, Inc.", "Memory Corp NV",
 "SiliconBlue Technologies", "Rambus Inc.", "Andes Technology Corporation", "Coronis Systems",
 "Achronix Semiconductor", "Siano Mobile Silicon Ltd.", "Semtech Corporation", "Pixelworks Inc.",
 "Gaisler Research AB", "Teranetics", "Toppan Printing Co. Ltd.", "Kingxcon",
 "Silicon Integrated Systems", "I-O Data Device, Inc.", "NDS Americas Inc.", "Solomon Systech Limited",
 "On Demand Microelectronics", "Amicus Wireless Inc.", "SMARDTV SNC", "Comsys Communication Ltd.",
 "Movidia Ltd.", "Javad GNSS, Inc.", "Montage Technology Group", "Trident Microsystems", "Super Talent",
 "Optichron, Inc.", "Future Waves UK Ltd.", "SiBEAM, Inc.", "Inicore, Inc.", "Virident Systems",
 "M2000, Inc.", "ZeroG Wireless, Inc.", "Gingle Technology Co. Ltd.", "Space Micro Inc.", "Wilocity",
 "Novafora, Inc.", "iKoa Corporation", "ASint Technology", "Ramtron", "Plato Networks Inc.",
 "IPtronics AS", "Infinite-Memories", "Parade Technologies Inc.", "Dune Networks",
 "GigaDevice Semiconductor", "Modu Ltd.", "CEITEC", "Northrop Grumman", "XRONET Corporation",
 "Sicon Semiconductor AB", "Atla Electronics Co. Ltd.", "TOPRAM Technology", "Silego Technology Inc.",
 "Kinglife", "Ability Industries Ltd.", "Silicon Power Computer & Communications",
 "Augusta Technology, Inc.", "Nantronics Semiconductors", "Hilscher Gesellschaft", "Quixant Ltd.",
 "Percello Ltd.", "NextIO Inc.", "Scanimetrics Inc.", "FS-Semi Company Ltd.", "Infinera Corporation",
 "SandForce Inc.", "Lexar Media", "Teradyne Inc.", "Memory Exchange Corp.", "Suzhou Smartek Electronics",
 "Avantium Corporation", "ATP Electronics Inc.", "Valens Semiconductor Ltd", "Agate Logic, Inc.",
 "Netronome", "Zenverge, Inc.", "N-trig Ltd", "SanMax Technologies Inc.", "Contour Semiconductor Inc.",
 "TwinMOS", "Silicon Systems, Inc.", "V-Color Technology Inc.", "Certicom Corporation", "JSC ICC Milandr",
 "PhotoFast Global Inc.", "InnoDisk Corporation", "Muscle Power", "Energy Micro", "Innofidei",
 "CopperGate Communications", "Holtek Semiconductor Inc.", "Myson Century, Inc.", "FIDELIX",
 "Red Digital Cinema", "Densbits Technology", "Zempro", "MoSys", "Provigent", "Triad Semiconductor, Inc."},
{"Siklu Communication Ltd.", "A Force Manufacturing Ltd.", "Strontium", "ALi Corp (former Abilis Systems)", "Siglead, Inc.",
 "Ubicom, Inc.", "Unifosa Corporation", "Stretch, Inc.", "Lantiq Deutschland GmbH", "Visipro",
 "EKMemory", "Microelectronics Institute ZTE", "u-blox AG (former Cognovo Ltd)", "Carry Technology Co. Ltd.", "Nokia",
 "King Tiger Technology", "Sierra Wireless", "HT Micron", "Albatron Technology Co. Ltd.",
 "Leica Geosystems AG", "BroadLight", "AEXEA", "ClariPhy Communications, Inc.", "Green Plug",
 "Design Art Networks", "Mach Xtreme Technology Ltd.", "ATO Solutions Co. Ltd.", "Ramsta",
 "Greenliant Systems, Ltd.", "Teikon", "Antec Hadron", "NavCom Technology, Inc.",
 "Shanghai Fudan Microelectronics", "Calxeda, Inc.", "JSC EDC Electronics", "Kandit Technology Co. Ltd.",
 "Ramos Technology", "Goldenmars Technology", "XeL Technology Inc.", "Newzone Corporation",
 "ShenZhen MercyPower Tech", "Nanjing Yihuo Technology", "Nethra Imaging Inc.", "SiTel Semiconductor BV",
 "SolidGear Corporation", "Topower Computer Ind Co Ltd.", "Wilocity", "Profichip GmbH",
 "Gerad Technologies", "Ritek Corporation", "Gomos Technology Limited", "Memoright Corporation",
 "D-Broad, Inc.", "HiSilicon Technologies", "Syndiant Inc.", "Enverv Inc.", "Cognex",
 "Xinnova Technology Inc.", "Ultron AG", "Concord Idea Corporation", "AIM Corporation",
 "Lifetime Memory Products", "Ramsway", "Recore Systems BV", "Haotian Jinshibo Science Tech",
 "Being Advanced Memory", "Adesto Technologies", "Giantec Semiconductor, Inc.", "HMD Electronics AG",
 "Gloway International (HK)", "Kingcore", "Anucell Technology Holding",
 "Accord Software & Systems Pvt. Ltd.", "Active-Semi Inc.", "Denso Corporation", "TLSI Inc.",
 "Shenzhen Daling Electronic Co. Ltd.", "Mustang", "Orca Systems", "Passif Semiconductor",
 "GigaDevice Semiconductor (Beijing) Inc.", "Memphis Electronic", "Beckhoff Automation GmbH",
 "Harmony Semiconductor Corp (former ProPlus Design Solutions)", "Air Computers SRL", "TMT Memory",
 "Eorex Corporation", "Xingtera", "Netsol", "Bestdon Technology Co. Ltd.", "Baysand Inc.",
 "Uroad Technology Co. Ltd. (former Triple Grow Industrial Ltd.)", "Wilk Elektronik S.A.",
 "AAI", "Harman", "Berg Microelectronics Inc.", "ASSIA, Inc.", "Visiontek Products LLC",
 "OCMEMORY", "Welink Solution Inc.", "Shark Gaming", "Avalanche Technology",
 "R&D Center ELVEES OJSC", "KingboMars Technology Co. Ltd.",
 "High Bridge Solutions Industria Eletronica", "Transcend Technology Co. Ltd.",
 "Everspin Technologies", "Hon-Hai Precision", "Smart Storage Systems", "Toumaz Group",
 "Zentel Electronics Corporation", "Panram International Corporation",
 "Silicon Space Technology", "LITE-ON IT Corporation", "Inuitive", "HMicro",
 "BittWare Inc.", "GLOBALFOUNDRIES", "ACPI Digital Co. Ltd", "Annapurna Labs",
 "AcSiP Technology Corporation", "Idea! Electronic Systems", "Gowe Technology Co. Ltd",
 "Hermes Testing Solutions Inc.", "Positivo BGH", "Intelligence Silicon Technology"},
{"3D PLUS", "Diehl Aerospace", "Fairchild", "Mercury Systems",
 "Sonics Inc.", "Emerson Automation Solutions (former ICC/GE Intelligent Platforms)", "Shenzhen Jinge Information Co. Ltd",
 "SCWW", "Silicon Motion Inc.", "Anurag", "King Kong",
 "FROM30 Co. Ltd", "Gowin Semiconductor Corp", "Fremont Micro Devices Ltd",
 "Ericsson Modems", "Exelis", "Satixfy Ltd", "Galaxy Microsystems Ltd",
 "Gloway International Co. Ltd", "Lab", "Smart Energy Instruments",
 "Approved Memory Corporation", "Axell Corporation", "Essencore Limited (former ISD Technology Limited)",
 "Phytium", "Xi’an UniIC Semiconductors Co Ltd (former Xi'an SinoChip Semiconductor)", "Ambiq Micro", "eveRAM Technology Inc.",
 "Infomax", "Butterfly Network Inc.", "Shenzhen City Gcai Electronics",
 "Stack Devices Corporation", "ADK Media Group", "TSP Global Co. Ltd",
 "HighX", "Shenzhen Elicks Technology", "ISSI/Chingis", "Google Inc.",
 "Dasima International Development", "Leahkinn Technology Limited",
 "HIMA Paul Hildebrandt GmbH Co KG", "Keysight Technologies",
 "Techcomp International (Fastable)", "Ancore Technology Corporation",
 "Nuvoton", "Korea Uhbele International Group Ltd", "Ikegami Tsushinki Co. Ltd",
 "RelChip Inc.", "Baikal Electronics", "Nemostech Inc.",
 "Memorysolution GmbH", "Silicon Integrated Systems Corporation",
 "Xiede", "BRC (former Multilaser Components)", "Flash Chi", "Jone",
 "GCT Semiconductor Inc.", "Hong Kong Zetta Device Technology",
 "Unimemory Technology(s) Pte Ltd", "Cuso", "Kuso",
 "Uniquify Inc.", "Skymedi Corporation", "Core Chance Co. Ltd",
 "Tekism Co. Ltd", "Seagate Technology PLC", "Hong Kong Gaia Group Co. Limited",
 "Gigacom Semiconductor LLC", "V2 Technologies", "TLi", "Neotion",
 "Lenovo", "Shenzhen Zhongteng Electronic Corp. Ltd", "Compound Photonics",
 "In2H2 Inc (former Cognimem Technologies Inc)", "Shenzhen Pango Microsystems Co. Ltd",
 "Vasekey", "Cal-Comp Industria de Semicondutores", "Eyenix Co. Ltd",
 "Heoriady", "Accelerated Memory Production Inc.", "INVECAS Inc.",
 "AP Memory", "Douqi Technology", "Etron Technology Inc.",
 "Indie Semiconductor", "Socionext Inc.", "HGST", "EVGA",
 "Audience Inc.", "EpicGear", "Vitesse Enterprise Co.",
 "Foxtronn International Corporation", "Bretelon Inc.", "Graphcore", "Eoplex Inc",
 "MaxLinear Inc", "ETA Devices", "LOKI", "IMS Electronics Co Ltd",
 "Dosilicon Co Ltd", "Dolphin Integration", "Shenzhen Mic Electronics Technolog",
 "Boya Microelectronics Inc", "Geniachip (Roche)", "Axign", "Kingred Electronic Technology Ltd",
 "Chao Yue Zhuo Computer Business Dept.", "Guangzhou Si Nuo Electronic Technology.",
 "Crocus Technology Inc", "Creative Chips GmbH", "GE Aviation Systems LLC.",
 "Asgard", "Good Wealth Technology Ltd", "TriCor Technologies", "Nova-Systems GmbH",
 "JUHOR", "Zhuhai Douke Commerce Co Ltd", "DSL Memory", "Anvo-Systems Dresden GmbH",
 "Realtek", "AltoBeam", "Wave Computing", "Beijing TrustNet Technology Co Ltd",
 "Innovium Inc", "Starsway Technology Limited"},
{"Weltronics Co LTD", "VMware Inc", "Hewlett Packard Enterprise", "INTENSO",
 "Puya Semiconductor", "MEMORFI", "MSC Technologies GmbH", "Txrui",
 "SiFive Inc", "Spreadtrum Communications", "XTX Technology Limited",
 "UMAX Technology", "Shenzhen Yong Sheng Technology", "SNOAMOO (Shenzhen Kai Zhuo Yue)",
 "Daten Tecnologia LTDA", "Shenzhen XinRuiYan Electronics", "Eta Compute",
 "Energous", "Raspberry Pi Trading Ltd", "Shenzhen Chixingzhe Tech Co Ltd",
 "Silicon Mobility", "IQ-Analog Corporation", "Uhnder Inc", "Impinj",
 "DEPO Computers", "Nespeed Sysems", "Yangtze Memory Technologies Co Ltd",
 "MemxPro Inc", "Tammuz Co Ltd", "Allwinner Technology",
 "Shenzhen City Futian District Qing Xuan Tong Computer Trading Firm", "XMC",
 "Teclast", "Maxsun", "Haiguang Integrated Circuit Design", "RamCENTER Technology",
 "Phison Electronics Corporation", "Guizhou Huaxintong Semi-Conductor",
 "Network Intelligence", "Continental Technology (Holdings)",
 "Guangzhou Huayan Suning Electronic", "Guangzhou Zhouji Electronic Co Ltd",
 "Shenzhen Giant Hui Kang Tech Co Ltd", "Shenzhen Yilong Innovative Co Ltd",
 "Neo Forza", "Lyontek Inc", "Shanghai Kuxin Microelectronics Ltd",
 "Shenzhen Larix Technology Co Ltd", "Qbit Semiconductor Ltd",
 "Insignis Technology Corporation", "Lanson Memory Co Ltd",
 "Shenzhen Superway Electronics Co Ltd", "Canaan-Creative Co Ltd",
 "Black Diamond Memory", "Shenzhen City Parker Baking Electronics",
 "Shenzhen Baihong Technology Co Ltd", "GEO Semiconductors", "OCPC", "Artery Technology Co Ltd",
 "Jinyu", "ShenzhenYing Chi Technology Development", "Shenzhen Pengcheng Xin Technology",
 "Pegasus Semiconductor (Shanghai) Co", "Mythic Inc", "Elmos Semiconductor AG",
 "Kllisre", "Shenzhen Winconway Technology", "Shenzhen Xingmem Technology Corp",
 "Gold Key Technology Co Ltd", "Habana Labs Ltd", "Hoodisk Electronics Co Ltd",
 "SemsoTai (SZ) Technology Co Ltd", "OM Nanotech Pvt. Ltd",
 "Shenzhen Zhifeng Weiye Technology", "Xinshirui (Shenzhen) Electronics Co",
 "Guangzhou Zhong Hao Tian Electronic", "Shenzhen Longsys Electronics Co Ltd",
 "Deciso B.V.", "Puya Semiconductor (Shenzhen)", "Shenzhen Veineda Technology Co Ltd",
 "Antec Memory", "Cortus SAS", "Dust Leopard", "MyWo AS",
 "J&A Information Inc", "Shenzhen JIEPEI Technology Co Ltd", "Heidelberg University",
 "Flexxon PTE Ltd", "Wiliot", "Raysun Electronics International Ltd",
 "Aquarius Production Company LLC", "MACNICA DHW LTDA", "Intelimem",
 "Zbit Semiconductor Inc", "Shenzhen Technology Co Ltd", "Signalchip",
 "Shenzen Recadata Storage Technology", "Hyundai Technology",
 "Shanghai Fudi Investment Development", "Aixi Technology", "Tecon MT",
 "Onda Electric Co Ltd", "Jinshen", "Kimtigo Semiconductor (HK) Limited",
 "IIT Madras", "Shenshan (Shenzhen) Electronic", "Hefei Core Storage Electronic Limited",
 "Colorful Technology Ltd", "Visenta (Xiamen) Technology Co Ltd", "Roa Logic BV",
 "NSITEXE Inc", "Hong Kong Hyunion Electronics", "ASK Technology Group Limited",
 "GIGA-BYTE Technology Co Ltd", "Terabyte Co Ltd", "Hyundai Inc", "EXCELERAM",
 "PsiKick", "Netac Technology Co Ltd", "PCCOOLER", "Jiangsu Huacun Electronic Technology",
 "Shenzhen Micro Innovation Industry", "Beijing Tongfang Microelectronics Co",
 "XZN Storage Technology", "ChipCraft Sp. z.o.o.", "ALLFLASH Technology Limited"},
{"Foerd Technology Co Ltd", "KingSpec", "Codasip GmbH", "SL Link Co Ltd",
 "Shenzhen Kefu Technology Co Limited", "Shenzhen ZST Electronics Technology",
 "Kyokuto Electronic Inc", "Warrior Technology", "TRINAMIC Motion Control GmbH & Co",
 "PixelDisplay Inc", "Shenzhen Futian District Bo Yueda Elec", "Richtek Power",
 "Shenzhen LianTeng Electronics Co Ltd", "AITC Memory", "UNIC Memory Technology Co Ltd",
 "Shenzhen Huafeng Science Technology", "CXMT (former Innotron Memory Co Ltd)",
 "Guangzhou Xinyi Heng Computer Trading Firm", "SambaNova Systems", "V-GEN",
 "Jump Trading", "Ampere Computing", "Shenzhen Zhongshi Technology Co Ltd",
 "Shenzhen Zhongtian Bozhong Technology", "Tri-Tech International",
 "Silicon Intergrated Systems Corporation", "Shenzhen HongDingChen Information",
 "Plexton Holdings Limited", "AMS (Jiangsu Advanced Memory Semi)",
 "Wuhan Jing Tian Interconnected Tech Co", "Axia Memory Technology",
 "Chipset Technology Holding Limited", "Shenzhen Xinshida Technology Co Ltd",
 "Shenzhen Chuangshifeida Technology", "Guangzhou MiaoYuanJi Technology", "ADVAN Inc",
 "Shenzhen Qianhai Weishengda Electronic Commerce Company Ltd", "Guangzhou Guang Xie Cheng Trading",
 "StarRam International Co Ltd", "Shen Zhen XinShenHua Tech Co Ltd",
 "UltraMemory Inc", "New Coastline Global Tech Industry Co", "Sinker", "Diamond",
 "PUSKILL", "Guangzhou Hao Jia Ye Technology Co", "Ming Xin Limited", "Barefoot Networks",
 "Biwin Semiconductor (HK) Co Ltd", "UD INFO Corporation", "Trek Technology (S) PTE Ltd",
 "Xiamen Kingblaze Technology Co Ltd", "Shenzhen Lomica Technology Co Ltd",
 "Nuclei System Technology Co Ltd", "Wuhan Xun Zhan Electronic Technology",
 "Shenzhen Ingacom Semiconductor Ltd", "Zotac Technology Ltd", "Foxline",
 "Shenzhen Farasia Science Technology", "Efinix Inc", "Hua Nan San Xian Technology Co Ltd",
 "Goldtech Electronics Co Ltd", "Shanghai Han Rong Microelectronics Co",
 "Shenzhen Zhongguang Yunhe Trading", "Smart Shine(QingDao) Microelectronics",
 "Thermaltake Technology Co Ltd", "Shenzhen O'Yang Maile Technology Ltd", "UPMEM",
 "Chun Well Technology Holding Limited", "Astera Labs Inc", "Winconway",
 "Advantech Co Ltd", "Chengdu Fengcai Electronic Technology", "The Boeing Company",
 "Blaize Inc", "Ramonster Technology Co Ltd", "Wuhan Naonongmai Technology Co Ltd",
 "Shenzhen Hui ShingTong Technology", "Yourlyon", "Fabu Technology",
 "Shenzhen Yikesheng Technology Co Ltd", "NOR-MEM", "Cervoz Co Ltd",
 "Bitmain Technologies Inc", "Facebook Inc", "Shenzhen Longsys Electronics Co Ltd",
 "Guangzhou Siye Electronic Technology", "Silergy", "Adamway",
 "PZG", "Shenzhen King Power Electronics", "Guangzhou ZiaoFu Tranding Co Ltd",
 "Shenzhen SKIHOTAR Semiconductor", "PulseRain Technology", "Seeker Technology Limited",
 "Shenzhen OSCOO Tech Co Ltd", "Shenzhen Yze Technology Co Ltd",
 "Shenzhen Jieshuo Electronic Commerce", "Gazda", "Hua Wei Technology Co Ltd",
 "Esperanto Technologies", "JinSheng Electronic (Shenzhen) Co Ltd",
 "Shenzhen Shi Bolunshuai Technology", "Shanghai Rei Zuan Information Tech",
 "Fraunhofer IIS", "Kandou Bus SA", "Acer", "Artmem Technology Co Ltd",
 "Gstar Semiconductor Co Ltd", "ShineDisk", "Shenzhen CHN Technology Co Ltd",
 "UnionChip Semiconductor Co Ltd", "Tanbassh", "Shenzhen Tianyu Jieyun Intl Logistics",
 "MCLogic Inc", "Eorex Corporation", "Arm Technology (China) Co Ltd", "Lexar Co Limited",
 "QinetiQ Group PLC", "Exascend", "Hong Kong Hyunion Electronics Co Ltd",
 "Shenzhen Banghong Electronics Co Ltd", "MBit Wireless Inc", "Hex Five Security Inc",
 "ShenZhen Juhor Precision Tech Co Ltd", "Shenzhen Reeinno Technology Co Ltd"},
{"ABIT Electronics (Shenzhen) Co Ltd", "Semidrive", "MyTek Electronics Corp",
 "Wxilicon Technology Co Ltd", "Shenzhen Meixin Electronics Ltd", "Ghost Wolf",
 "LiSion Technologies Inc", "Power Active Co Ltd", "Pioneer High Fidelity Taiwan Co. Ltd",
 "LuoSilk", "Shenzhen Chuangshifeida Technology", "Black Sesame Technologies Inc",
 "Jiangsu Xinsheng Intelligent Technology", "MLOONG", "Quadratica LLC",
 "Anpec Electronics", "Xi’an Morebeck Semiconductor Tech Co", "Kingbank Technology Co Ltd",
 "ITRenew Inc", "Shenzhen Eaget Innovation Tech Ltd", "Jazer",
 "Xiamen Semiconductor Investment Group", "Guangzhou Longdao Network Tech Co",
 "Shenzhen Futian SEC Electronic Market", "Allegro Microsystems LLC",
 "Hunan RunCore Innovation Technology", "C-Corsa Technology",
 "Zhuhai Chuangfeixin Technology Co Ltd", "Beijing InnoMem Technologies Co Ltd",
 "YooTin", "Shenzhen Pengxiong Technology Co Ltd", "Dongguan Yingbang Commercial Trading Co",
 "Shenzhen Ronisys Electronics Co Ltd", "Hongkong Xinlan Guangke Co Ltd",
 "Apex Microelectronics Co Ltd", "Beijing Hongda Jinming Technology Co Ltd",
 "Ling Rui Technology (Shenzhen) Co Ltd", "Hongkong Hyunion Electronics Co Ltd",
 "Starsystems Inc", "Shenzhen Yingjiaxun Industrial Co Ltd",
 "Dongguan Crown Code Electronic Commerce", "Monolithic Power Systems Inc",
 "WuHan SenNaiBo E-Commerce Co Ltd", "Hangzhou Hikstorage Technology Co",
 "Shenzhen Goodix Technology Co Ltd", "Aigo Electronic Technology Co Ltd",
 "Hefei Konsemi Storage Technology Co Ltd", "Cactus Technologies Limited",
 "DSIN", "Blu Wireless Technology", "Nanjing UCUN Technology Inc",
 "Acacia Communications", "Beijinjinshengyihe Technology Co Ltd", "Zyzyx",
 "T-HEAD Semiconductor Co Ltd", "Shenzhen Hystou Technology Co Ltd", "Syzexion",
 "Kembona", "Qingdao Thunderobot Technology Co Ltd", "Morse Micro",
 "Shenzhen Envida Technology Co Ltd", "UDStore Solution Limited", "Shunlie",
 "Shenzhen Xin Hong Rui Tech Ltd", "Shenzhen Yze Technology Co Ltd",
 "Shenzhen Huang Pu He Xin Technology", "Xiamen Pengpai Microelectronics Co Ltd",
 "JISHUN", "Shenzhen WODPOSIT Technology Co", "Unistar", "UNICORE Electronic (Suzhou) Co Ltd",
 "Axonne Inc", "Shenzhen SOVERECA Technology Co", "Dire Wolf", "Whampoa Core Technology Co Ltd",
 "CSI Halbleiter GmbH", "ONE Semiconductor", "SimpleMachines Inc",
 "Shenzhen Chengyi Qingdian Electronic", "Shenzhen Xinlianxin Network Technology",
 "Vayyar Imaging Ltd", "Paisen Network Technology Co Ltd",
 "Shenzhen Fengwensi Technology Co Ltd", "Caplink Technology Limited", "JJT Solution Co Ltd",
 "HOSIN Global Electronics Co Ltd", "Shenzhen KingDisk Century Technology", "SOYO",
 "DIT Technology Co Ltd", "iFound", "Aril Computer Company", "ASUS",
 "Shenzhen Ruiyingtong Technology Co", "HANA Micron", "RANSOR", "Axiado Corporation",
 "Tesla Corporation", "Pingtouge (Shanghai) Semiconductor Co", "S3Plus Technologies SA",
 "Integrated Silicon Solution Israel Ltd", "GreenWaves Technologies", "NUVIA Inc",
 "Guangzhou Shuvrwine Technology Co", "Shenzhen Hangshun Chip Technology",
 "Chengboliwei Electronic Business", "Kowin Memory Technology Co Ltd", "Euronet Technology Inc",
 "SCY", "Shenzhen Xinhongyusheng Electrical", "PICOCOM", "Shenzhen Toooogo Memory Technology",
 "VLSI Solution", "Costar Electronics Inc", "Shenzhen Huatop Technology Co Ltd",
 "Inspur Electronic Information Industry", "Shenzhen Boyuan Computer Technology",
 "Beijing Welldisk Electronics Co Ltd", "Suzhou EP Semicon Co Ltd",
 "Zhejiang Dahua Memory Technology", "Virtu Financial", "Datotek International Co Ltd",
 "Telecom and Microelectronics Industries", "Echo Technology Ltd", "APEX-INFO",
 "Yingpark", "Shenzhen Bigway Tech Co Ltd"},
{"Beijing Haawking Technology Co Ltd", "Open HW Group", "JHICC", "ncoder AG",
 "ThinkTech Information Technology Co", "Shenzhen Chixingzhe Technology Co Ltd",
 "Skywalker", "Shenzhen Kaizhuoyue Electronics Co Ltd",
 "Shenzhen YC Storage Technology Co Ltd", "Shenzhen Chixingzhe Technology Co",
 "Wink Semiconductor (Shenzhen) Co Ltd", "AISTOR", "Palma Ceia SemiDesign",
 "EM Microelectronic-Marin SA", "Shenzhen Monarch Memory Technology"}
};
