import io, sys, os

allowed_sections = {
    ".init": True,
    ".text": True,
    ".fini": True,
    ".rodata": True,
    ".data": True,
    ".sdata": True,
    ".sdata2": True,
}

def parse_map(mapfile:io.TextIOWrapper, dolphinmapfile:io.TextIOWrapper, start_address:int):
    is_section_allowed = False
    memory_map_found = False

    line = mapfile.readline()

    while line:
        line = line.strip()
        if line == "Linker script and memory map":
            memory_map_found = True
            break

        line = mapfile.readline()

    if not memory_map_found:
        raise Exception("No memory map found.")

    line = mapfile.readline()

    while line:
        linelist = line.split()

        # change to new section
        if line[:1] == ".":
            is_section_allowed = allowed_sections.get(linelist[0], False)
            
            if is_section_allowed:

                # write out new section header
                section_header = "{} section layout\n".format(linelist[0])
                dolphinmapfile.write(section_header)

        elif is_section_allowed:
            if len(linelist) == 2:

                # symbol entry
                if linelist[0][:2] == "0x" and linelist[1][:2] != "0x":
                    address = int(linelist[0], 16)
                    address += start_address
                    address = hex(address)[2:]
                    symbol_entry = "{} {}\n".format(address, linelist[1])
                    dolphinmapfile.write(symbol_entry)

        line = mapfile.readline()

def parse_for_dol(parentdir, dolphinmapfile:io.TextIOWrapper):
    mapfile = open(parentdir + "/main.map")

    parse_map(mapfile, dolphinmapfile, 0)

    mapfile.close()

def parse_for_rel(parentdir, dolphinmapfile:io.TextIOWrapper):
    dolfile = open(parentdir + "/main.dol", "rb")
    relfile = open(parentdir + "/_Main.rel", "rb")
    mapfile = open(parentdir + "/_Main.map")

    dolfile.seek(0xD8)
    start_address = int.from_bytes(dolfile.read(4), 'big')
    bss_size = int.from_bytes(dolfile.read(4), 'big')

    relfile.seek(0x54)
    code_section_start = int.from_bytes(relfile.read(4), 'big')
    code_section_start &= ~1 # remove executable bit flag
    start_address += (bss_size + 0x1EED40 + code_section_start)

    relfile.close()
    dolfile.close()

    parse_map(mapfile, dolphinmapfile, start_address)

    mapfile.close()

buildDir = "../build"
if len(sys.argv) > 1:
    buildDir = sys.argv[1]
buildDir = os.path.normpath(buildDir)

dolphinMapFile = open("GXSRTE.map", "w")

dolphinMapFile.write("-- main.dol memory map --\n\n")
parse_for_dol(buildDir, dolphinMapFile)

#dolphinMapFile.write("\n-- _Main.rel memory map --\n\n")
#parse_for_rel(buildDir, dolphinMapFile)

dolphinMapFile.close()