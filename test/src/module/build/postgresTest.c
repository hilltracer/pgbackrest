/***********************************************************************************************************************************
Test Build PostgreSQL Interface
***********************************************************************************************************************************/
#include "common/harnessStorage.h"

/***********************************************************************************************************************************
Test Run
***********************************************************************************************************************************/
static void
testRun(void)
{
    FUNCTION_HARNESS_VOID();

    // Create default storage object for testing
    Storage *storageTest = storagePosixNewP(TEST_PATH_STR, .write = true);

    // *****************************************************************************************************************************
    if (testBegin("bldPgParse() and bldPg*Render()"))
    {
        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("parse errors");

        HRN_STORAGE_PUT_Z(
            storageTest, "src/postgres/interface/version.vendor.h",
            "#define CATALOG_VERSION_NO\t1\n"
            "#define PG_CONTROL_VERSION  2\n"
            "\n"
            "typedef int64_t int64;\n"
            "\n"
            "typedef struct struct_type\n"
            "{\n"
            "    int field1;\n"
            "    int field2;\n"
            "} struct_type;\n"
            "\n"
            "typedef enum\n"
            "{\n"
            "    enum1 = 0,\n"
            "    enum2,\n"
            "} enum_type;\n");

        HRN_STORAGE_PUT_Z(
            storageTest, "src/postgres/interface/version.intern.h",
            "#define  PG_INTERFACE_CONTROL_IS(version)\n");

        TEST_ERROR(
            bldPgParse(storageTest), FormatError,
            "unable to find define -- are there extra spaces on '#define  PG_INTERFACE_CONTROL_IS(version)'");

        HRN_STORAGE_PUT_Z(
            storageTest, "src/postgres/interface/version.intern.h",
            "#define PG_INTERFACE_CONTROL_IS(version)\n"
            "#define PG_INTERFACE_CONTROL(version)\n");

        HRN_STORAGE_PUT_Z(
            storageTest, "src/build/postgres/postgres.yaml",
            "bogus: value\n");

        HRN_STORAGE_PUT_Z(
            storageTest, "src/build/postgres/postgres.yaml",
            "bogus: value\n");

        TEST_ERROR(bldPgParse(storageTest), FormatError, "unknown postgres definition 'bogus'");

        HRN_STORAGE_PUT_Z(
            storageTest, "src/build/postgres/postgres.yaml",
            "version:\n"
            "  - 11:\n"
            "      bogus: value");

        TEST_ERROR(bldPgParse(storageTest), FormatError, "unknown postgres definition 'bogus'");

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("parse and render postgres");

        HRN_STORAGE_PUT_Z(
            storageTest, "src/build/postgres/postgres.yaml",
            "version:\n"
            "  - 9.6\n"
            "  - 9.4:\n"
            "      fork: GPDB\n"
            "      release: false\n"
            "  - 12:\n"
            "      fork: GPDB\n"
            "      release: false\n"
            "  - 11:\n"
            "      release: false\n");

        TEST_RESULT_VOID(bldPgRender(storageTest, bldPgParse(storageTest)), "parse and render");
        TEST_RESULT_VOID(bldPgVersionRender(storageTest, bldPgParse(storageTest)), "parse and render");

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("check version.auto.h");

        TEST_STORAGE_GET(
            storageTest,
            "src/postgres/version.auto.h",
            COMMENT_BLOCK_BEGIN "\n"
            "PostgreSQL Version\n"
            "\n"
            "Automatically generated by 'build-code postgres-version' -- do not modify directly.\n"
            COMMENT_BLOCK_END "\n"
            "#ifndef POSTGRES_VERSION_AUTO_H\n"
            "#define POSTGRES_VERSION_AUTO_H\n"
            "\n"
            COMMENT_BLOCK_BEGIN "\n"
            "PostgreSQL version constants\n"
            COMMENT_BLOCK_END "\n"
            "#define PG_VERSION_96                                               90600\n"
            "#define PG_VERSION_94                                               90400\n"
            "#define PG_VERSION_12                                               120000\n"
            "#define PG_VERSION_11                                               110000\n"
            "\n"
            "#define PG_VERSION_MAX                                              PG_VERSION_11\n"
            "\n"
            COMMENT_BLOCK_BEGIN "\n"
            "PostgreSQL version string constants for use in error messages\n"
            COMMENT_BLOCK_END "\n"
            "#define PG_VERSION_96_Z                                             \"9.6\"\n"
            "#define PG_VERSION_94_Z                                             \"9.4\"\n"
            "#define PG_VERSION_12_Z                                             \"12\"\n"
            "#define PG_VERSION_11_Z                                             \"11\"\n"
            "\n"
            "#endif\n");

        // -------------------------------------------------------------------------------------------------------------------------
        TEST_TITLE("check interface.auto.c.inc");

        TEST_STORAGE_GET(
            storageTest,
            "src/postgres/interface.auto.c.inc",
            COMMENT_BLOCK_BEGIN "\n"
            "PostgreSQL Interface\n"
            "\n"
            "Automatically generated by 'build-code postgres' -- do not modify directly.\n"
            COMMENT_BLOCK_END "\n"
            "\n"
            COMMENT_BLOCK_BEGIN "\n"
            "PostgreSQL 11 interface\n"
            COMMENT_BLOCK_END "\n"
            "#define PG_VERSION                                                  PG_VERSION_11\n"
            "#define FORK_POSTGRESQL\n"
            "\n"
            "#define enum1                                                       enum1_11PostgreSQL\n"
            "#define enum2                                                       enum2_11PostgreSQL\n"
            "#define enum_type                                                   enum_type_11PostgreSQL\n"
            "#define int64                                                       int64_11PostgreSQL\n"
            "#define struct_type                                                 struct_type_11PostgreSQL\n"
            "\n"
            "#define CATALOG_VERSION_NO_MAX\n"
            "\n"
            "#include \"postgres/interface/version.intern.h\"\n"
            "\n"
            "PG_INTERFACE_CONTROL_IS(11PostgreSQL);\n"
            "PG_INTERFACE_CONTROL(11PostgreSQL);\n"
            "\n"
            "#undef enum1\n"
            "#undef enum2\n"
            "#undef enum_type\n"
            "#undef int64\n"
            "#undef struct_type\n"
            "\n"
            "#undef CATALOG_VERSION_NO\n"
            "#undef CATALOG_VERSION_NO_MAX\n"
            "#undef PG_CONTROL_VERSION\n"
            "#undef PG_VERSION\n"
            "\n"
            "#undef PG_INTERFACE_CONTROL_IS\n"
            "#undef PG_INTERFACE_CONTROL\n"
            "\n"
            "#undef FORK_POSTGRESQL\n"
            "\n"
            "\n"
            COMMENT_BLOCK_BEGIN "\n"
            "GPDB 12 interface\n"
            COMMENT_BLOCK_END "\n"
            "#define PG_VERSION                                                  PG_VERSION_12\n"
            "#define FORK_GPDB\n"
            "\n"
            "#define enum1                                                       enum1_12GPDB\n"
            "#define enum2                                                       enum2_12GPDB\n"
            "#define enum_type                                                   enum_type_12GPDB\n"
            "#define int64                                                       int64_12GPDB\n"
            "#define struct_type                                                 struct_type_12GPDB\n"
            "\n"
            "#define CATALOG_VERSION_NO_MAX\n"
            "\n"
            "#include \"postgres/interface/version.intern.h\"\n"
            "\n"
            "PG_INTERFACE_CONTROL_IS(12GPDB);\n"
            "PG_INTERFACE_CONTROL(12GPDB);\n"
            "\n"
            "#undef enum1\n"
            "#undef enum2\n"
            "#undef enum_type\n"
            "#undef int64\n"
            "#undef struct_type\n"
            "\n"
            "#undef CATALOG_VERSION_NO\n"
            "#undef CATALOG_VERSION_NO_MAX\n"
            "#undef PG_CONTROL_VERSION\n"
            "#undef PG_VERSION\n"
            "\n"
            "#undef PG_INTERFACE_CONTROL_IS\n"
            "#undef PG_INTERFACE_CONTROL\n"
            "\n"
            "#undef FORK_GPDB\n"
            "\n"
            "\n"
            COMMENT_BLOCK_BEGIN "\n"
            "GPDB 9.4 interface\n"
            COMMENT_BLOCK_END "\n"
            "#define PG_VERSION                                                  PG_VERSION_94\n"
            "#define FORK_GPDB\n"
            "\n"
            "#define enum1                                                       enum1_94GPDB\n"
            "#define enum2                                                       enum2_94GPDB\n"
            "#define enum_type                                                   enum_type_94GPDB\n"
            "#define int64                                                       int64_94GPDB\n"
            "#define struct_type                                                 struct_type_94GPDB\n"
            "\n"
            "#define CATALOG_VERSION_NO_MAX\n"
            "\n"
            "#include \"postgres/interface/version.intern.h\"\n"
            "\n"
            "PG_INTERFACE_CONTROL_IS(94GPDB);\n"
            "PG_INTERFACE_CONTROL(94GPDB);\n"
            "\n"
            "#undef enum1\n"
            "#undef enum2\n"
            "#undef enum_type\n"
            "#undef int64\n"
            "#undef struct_type\n"
            "\n"
            "#undef CATALOG_VERSION_NO\n"
            "#undef CATALOG_VERSION_NO_MAX\n"
            "#undef PG_CONTROL_VERSION\n"
            "#undef PG_VERSION\n"
            "\n"
            "#undef PG_INTERFACE_CONTROL_IS\n"
            "#undef PG_INTERFACE_CONTROL\n"
            "\n"
            "#undef FORK_GPDB\n"
            "\n"
            "\n"
            COMMENT_BLOCK_BEGIN "\n"
            "PostgreSQL 9.6 interface\n"
            COMMENT_BLOCK_END "\n"
            "#define PG_VERSION                                                  PG_VERSION_96\n"
            "#define FORK_POSTGRESQL\n"
            "\n"
            "#define enum1                                                       enum1_96PostgreSQL\n"
            "#define enum2                                                       enum2_96PostgreSQL\n"
            "#define enum_type                                                   enum_type_96PostgreSQL\n"
            "#define int64                                                       int64_96PostgreSQL\n"
            "#define struct_type                                                 struct_type_96PostgreSQL\n"
            "\n"
            "#include \"postgres/interface/version.intern.h\"\n"
            "\n"
            "PG_INTERFACE_CONTROL_IS(96PostgreSQL);\n"
            "PG_INTERFACE_CONTROL(96PostgreSQL);\n"
            "\n"
            "#undef enum1\n"
            "#undef enum2\n"
            "#undef enum_type\n"
            "#undef int64\n"
            "#undef struct_type\n"
            "\n"
            "#undef CATALOG_VERSION_NO\n"
            "#undef CATALOG_VERSION_NO_MAX\n"
            "#undef PG_CONTROL_VERSION\n"
            "#undef PG_VERSION\n"
            "\n"
            "#undef PG_INTERFACE_CONTROL_IS\n"
            "#undef PG_INTERFACE_CONTROL\n"
            "\n"
            "#undef FORK_POSTGRESQL\n"
            "\n"
            "\n"
            COMMENT_BLOCK_BEGIN "\n"
            "PostgreSQL interface struct\n"
            COMMENT_BLOCK_END "\n"
            "static const PgInterface pgInterface[] =\n"
            "{\n"
            "    {\n"
            "        .version = PG_VERSION_11,\n"
            "        .fork = CFGOPTVAL_FORK_POSTGRESQL,\n"
            "\n"
            "        .controlIs = pgInterfaceControlIs11PostgreSQL,\n"
            "        .control = pgInterfaceControl11PostgreSQL,\n"
            "    },\n"
            "    {\n"
            "        .version = PG_VERSION_12,\n"
            "        .fork = CFGOPTVAL_FORK_GPDB,\n"
            "\n"
            "        .controlIs = pgInterfaceControlIs12GPDB,\n"
            "        .control = pgInterfaceControl12GPDB,\n"
            "    },\n"
            "    {\n"
            "        .version = PG_VERSION_94,\n"
            "        .fork = CFGOPTVAL_FORK_GPDB,\n"
            "\n"
            "        .controlIs = pgInterfaceControlIs94GPDB,\n"
            "        .control = pgInterfaceControl94GPDB,\n"
            "    },\n"
            "    {\n"
            "        .version = PG_VERSION_96,\n"
            "        .fork = CFGOPTVAL_FORK_POSTGRESQL,\n"
            "\n"
            "        .controlIs = pgInterfaceControlIs96PostgreSQL,\n"
            "        .control = pgInterfaceControl96PostgreSQL,\n"
            "    },\n"
            "};\n");
    }

    FUNCTION_HARNESS_RETURN_VOID();
}
