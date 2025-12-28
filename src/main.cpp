#include <stdio.h>
#include <cstdint>
#include <sqlite3.h> //https://www.sqlite.org/cintro.html

class SQL_header {
	
	private:

		char header_string[16];
		uint16_t page_size;
		uint8_t write_version; //1 legacy, 2 WAL
		uint8_t read_version; // 1 legacy, 2 WAL
		uint8_t reserved_space; //usually 0
		uint8_t maximum_embedded_payload_fraction; //must be 64
		uint8_t minimum_embedded_payload_fraction; //must be 32
		uint8_t leaf_payload_fraction; //must be 32
		uint32_t file_change_counter;
		uint32_t size_of_database_file_in_pages;
		uint32_t page_number_of_first_freelist_trunk_page;
		uint32_t total_numer_of_freelist_pages;
		uint32_t schema_cookie;
		uint32_t schema_format_number;
		uint32_t default_page_cache_size;
		uint32_t page_number_of_largest_root_btree_page;
		uint32_t database_text_encoding; //1 - UTF-8
						 //2 - UTF-16le
						 //3 - UTF-16be
		uint32_t user_version;
		uint32_t incremental_vacuum_mode; //0 - false, non-zero - true
		uint32_t application_id;
		uint8_t  reserved_for_expansion[20]; //must be zero
		uint32_t version_valid_for_number;
		uint32_t sqlite_version_number;

	public:
		void print();
};

void SQL_header::print() {
	printf("Header String: %s\n", header_string);
	printf("page size: %u\n", page_size);
	printf("write version: %u\n", write_version);
	printf("read version: %u\n", read_version);
	printf("reserved_space: %u\n", reserved_space);
	printf("maximum embedded payload fraction(64): %u\n", maximum_embedded_payload_fraction);
	printf("minimum embedded payload fraction(32): %u\n", minimum_embedded_payload_fraction);
	printf("leaf payload fraction(32): %u\n", leaf_payload_fraction);
	printf("file change counter: %u\n", file_change_counter);
	printf("size of database file in pages: %u\n", size_of_database_file_in_pages);
	printf("page number of first freelist trunk page: %u\n", page_number_of_first_freelist_trunk_page);
	printf("total numer of freelist pages: %u\n", total_numer_of_freelist_pages);
	printf("schema cookie: %u\n", schema_cookie);
	printf("schema format number: %u\n", schema_format_number);
	printf("default page cache size: %u\n", default_page_cache_size);
	printf("page number of largest root b-tree page: %u\n", page_number_of_largest_root_btree_page);
	printf("database text encoding: %u\n", database_text_encoding);
	printf("user version: %u\n", user_version);
	printf("incremental vacuum mode: %u\n", incremental_vacuum_mode);
	printf("application id: %u\n", application_id);
	printf("version valid for number: %u\n", version_valid_for_number);
	printf("sqlite version number: %u\n", sqlite_version_number);

	printf("reserved_for_expansion(must be zero): ");
	for(int i=0; i < 20; i++) {
		printf("%u ", reserved_for_expansion[i]);
	}
	printf("\n");
}

int main(int argc, char** argv) {
	
	FILE* database_file;
	database_file = fopen("pga.db", "r");
	
	if(database_file == NULL) {
		printf("Error while opening file\n");
		return 1;
	}

	SQL_header database_header;

	fread( &database_header,1,100, database_file);

	database_header.print();

	fclose(database_file);
	database_file = NULL;

	return 0;
}

