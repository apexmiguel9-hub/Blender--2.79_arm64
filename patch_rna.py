#!/usr/bin/env python3
import sys
import os

def patch_file(filepath, patches):
    """Apply patches to a file."""
    with open(filepath, 'r') as f:
        content = f.read()
    
    for old, new in patches:
        if old in content:
            content = content.replace(old, new)
        else:
            print(f"WARNING: Could not find patch target in {filepath}")
            print(f"  Looking for: {old[:100]}...")
    
    with open(filepath, 'w') as f:
        f.write(content)

def main():
    base = "/home/runner/work/Blender3.6_arm64/Blender3.6_arm64/blender36"
    
    # Patch rna_internal.h - disable USE_RNA_RANGE_CHECK
    patch_file(
        os.path.join(base, "source/blender/makesrna/intern/rna_internal.h"),
        [("  define USE_RNA_RANGE_CHECK", "undef USE_RNA_RANGE_CHECK")]
    )
    
    # Add forward declaration for rna_lang_enum_properties_get_no_international
    with open(os.path.join(base, "source/blender/makesrna/intern/rna_internal.h"), 'r') as f:
        content = f.read()
    
    # Insert after the first #endif (after extern "C" block opening)
    idx = content.find("#endif\n\n#define RNA_MAGIC")
    if idx >= 0:
        insert_pos = idx + len("#endif")
        content = content[:insert_pos] + "\nint rna_lang_enum_properties_get_no_international(PointerRNA *ptr);\n" + content[insert_pos:]
    
    # Add forward declarations for FluidModifier functions
    idx2 = content.find("int rna_lang_enum_properties_get_no_international(PointerRNA *ptr);")
    if idx2 >= 0:
        insert_pos2 = idx2 + len("int rna_lang_enum_properties_get_no_international(PointerRNA *ptr);")
        fluid_decls = """
int rna_FluidModifier_grid_get_length(const PointerRNA *ptr, int *length);
int rna_FluidModifier_color_grid_get_length(const PointerRNA *ptr, int *length);
int rna_FluidModifier_velocity_grid_get_length(const PointerRNA *ptr, int *length);
int rna_FluidModifier_heat_grid_get_length(const PointerRNA *ptr, int *length);
void rna_FluidModifier_density_grid_get(PointerRNA *ptr, float *values);
void rna_FluidModifier_velocity_grid_get(PointerRNA *ptr, float *values);
void rna_FluidModifier_color_grid_get(PointerRNA *ptr, float *values);
void rna_FluidModifier_flame_grid_get(PointerRNA *ptr, float *values);
void rna_FluidModifier_heat_grid_get(PointerRNA *ptr, float *values);
void rna_FluidModifier_temperature_grid_get(PointerRNA *ptr, float *values);"""
        content = content[:idx2 + len("int rna_lang_enum_properties_get_no_international(PointerRNA *ptr);")] + "\n" + fluid_decls + content[idx2 + len("int rna_lang_enum_properties_get_no_international(PointerRNA *ptr);"):]
    
    with open(os.path.join(base, "source/blender/makesrna/intern/rna_internal.h"), 'w') as f:
        f.write(content)
    
    # Patch rna_userdef.c - remove static from rna_lang_enum_properties_get_no_international
    patch_file(
        os.path.join(base, "source/blender/makesrna/intern/rna_userdef.c"),
        [("static int rna_lang_enum_properties_get_no_international(", "int rna_lang_enum_properties_get_no_international(")]
    )
    
    # Patch rna_fluid.c - remove static from FluidModifier functions
    fluid_patches = [
        ("static int rna_FluidModifier_grid_get_length(", "int rna_FluidModifier_grid_get_length("),
        ("static int rna_FluidModifier_color_grid_get_length(", "int rna_FluidModifier_color_grid_get_length("),
        ("static int rna_FluidModifier_velocity_grid_get_length(", "int rna_FluidModifier_velocity_grid_get_length("),
        ("static int rna_FluidModifier_heat_grid_get_length(", "int rna_FluidModifier_heat_grid_get_length("),
        ("static void rna_FluidModifier_density_grid_get(", "void rna_FluidModifier_density_grid_get("),
        ("static void rna_FluidModifier_velocity_grid_get(", "void rna_FluidModifier_velocity_grid_get("),
        ("static void rna_FluidModifier_color_grid_get(", "void rna_FluidModifier_color_grid_get("),
        ("static void rna_FluidModifier_flame_grid_get(", "void rna_FluidModifier_flame_grid_get("),
        ("static void rna_FluidModifier_heat_grid_get(", "void rna_FluidModifier_heat_grid_get("),
        ("static void rna_FluidModifier_temperature_grid_get(", "void rna_FluidModifier_temperature_grid_get("),
    ]
    patch_file(os.path.join(base, "source/blender/makesrna/intern/rna_fluid.c"), fluid_patches)
    
    print("All patches applied successfully")

if __name__ == "__main__":
    main()