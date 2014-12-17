/*
 * tools/main.c
 * 
 * Copyright (c) 2011, old_fashioned. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */


#include <iso646.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#ifndef WIN32

#ifdef HAVE_CONFIG_H
	#include "config.h"
#endif

#ifndef NDEBUG
	#undef TEMPLATE_DIR
	#define	TEMPLATE_DIR	"../template"
#endif	/* NDEBUG */

#endif /* ndef WIN32 */

#ifdef WIN32
	#include "../config.h"

#ifdef NDEBUG
	char * template_dir;
	#define	TEMPLATE_DIR	template_dir
#else
	#define	TEMPLATE_DIR	"template"
#endif

#endif /* WIN32 */



enum
ErrorCode
{
	OOC_TOOL_OK		= 0,
	OOC_TOOL_ERROR_FEW_PARAMETERS,
	OOC_TOOL_ERROR_BAD_PARAMETER,
	OOC_TOOL_ERROR_CANT_OPEN_FILE,
	OOC_TOOL_ERROR_FILE_IO_PROBLEM
};

enum
OperationCode
{
	OPERATION_NONE	=	0,
	OPERATION_NEW,
	OPERATION_TESTSUITE
};

struct
ClassNameStruct
{
	char *	original;		/* The original name */	
	char *	lowercase;		/* Lowercase name */
	char *	uppercase;		/* uppercase name */
};

struct
SettingsStruct
{
	struct ClassNameStruct		new_class;			/* The new class to be created */
	struct ClassNameStruct		from_class;			/* The template class */
	struct ClassNameStruct		new_parent;			/* The new class' parent */
	struct ClassNameStruct		from_parent;		/* The template class' parent */
	
	char *						input_dir;			/* Input file's directory name */
	char *						input_file;			/* Input file name (base name only) */
	char *						subfolder;			/* The subfolder to be included in the search */
	
	char *						output_dir;			/* Output directory name */
	char * 						output_file;		/* Output file name (base name only) */ 
	
	gboolean					process_parent;		/* if the parent must be processed as well */
};

typedef struct SettingsStruct * Settings;

/* Global variables
 */

struct SettingsStruct			settings;
enum OperationCode				operation = OPERATION_NONE;

void
settings_set_defaults_I( Settings self )
{
	self->new_class.original	=	"";
	self->from_class.original	=	"Sample";
	self->new_parent.original	=	"Base";
	self->from_parent.original	=	"Base";
	self->subfolder				=	"implement";
}

void
settings_set_defaults_II( Settings self )
{
	self->new_class.lowercase 	=	g_ascii_strdown( self->new_class.original, -1 );
	self->new_class.uppercase 	=	g_ascii_strup(   self->new_class.original, -1 );

	self->from_class.lowercase 	=	g_ascii_strdown( self->from_class.original, -1 );
	self->from_class.uppercase 	=	g_ascii_strup(   self->from_class.original, -1 );

	self->new_parent.lowercase 	=	g_ascii_strdown( self->new_parent.original, -1 );
	self->new_parent.uppercase 	=	g_ascii_strup(   self->new_parent.original, -1 );

	self->from_parent.lowercase =	g_ascii_strdown( self->from_parent.original, -1 );
	self->from_parent.uppercase =	g_ascii_strup(   self->from_parent.original, -1 );
	
	if( not self->input_file )
		self->input_file = g_strdup( "sample" );
		
	if( not self->input_dir )
		self->input_dir = g_strdup( TEMPLATE_DIR );
		
	if( not self->output_file )
		self->output_file = g_strdup( self->new_class.lowercase );
		
	if( not self->output_dir )
		self->output_dir = g_strdup( "" );
		
	self->process_parent		=	( g_strcmp0( self->from_parent.original, self->new_parent.original ) != 0 );
}

void
settings_release( Settings self )
{
	g_free( self->new_class.lowercase );
	g_free( self->new_class.uppercase );

	g_free( self->from_class.lowercase );
	g_free( self->from_class.uppercase );

	g_free( self->new_parent.lowercase );
	g_free( self->new_parent.uppercase );

	g_free( self->from_parent.lowercase );
	g_free( self->from_parent.uppercase );
	
	g_free( self->output_dir );
	g_free( self->output_file );
	g_free( self->input_dir );
	g_free( self->input_file );
}

#if defined( WIN32 ) && defined( NDEBUG )
	char * win_template_dir( const char * executable )
	{
		char * td;
		char * full_path;
		char * exec_dir;
		
		full_path = g_find_program_in_path( executable );

		if( full_path )
		{
			exec_dir = g_path_get_dirname ( full_path );

			td = g_build_filename( exec_dir, "template", NULL );

			g_free( exec_dir );
		}
		g_free( full_path );

		return td;
	}
#endif

static 
void
print_help( void )
{
	char * help_msg = \
			"ooc tool - Helper for Object Oriented C programming - version %s\n"
			"(c) Tibor Miseta,  this software is licensed under GNU LGPL 3\n"
			"Use: ooc OPERATION [ MODIFIERS ]\n"
			"OPERATION can be:\n"
			"    -n ClassName\n"
			"    --new ClassName             Creates a ooc class in files classname.c\n"
			"                                and classname.h\n"
			"    --testsuite                 Creates a Unit test suite in the\n"
			"                                current (working) directory\n"
			"MODIFIERS for the --new command can be:\n"
			"    -p ParentClassName\n"
			"    --parent ParentClassName    Creates the class with this parent\n"
			"    -f SampleClassName\n"
			"    --from SampleClassName      Uses SampleClassName as a template\n"
			"    -t filename\n"
			"    --target filename           Puts the class in filename.c and\n"
			"                                filename.h files\n"
			"    -s filename\n"
			"    --source filename           Reads the template class from filename.c\n"
			"                                and filename.h files\n"
			"\n";

	g_print( help_msg, VERSION );	
}

int
get_args( int argc, char * argv[] )
{
	int i;
	
	if( argc < 2 ) {
		g_printerr( " Too few parameters. Use ooc --help for more info.\n" );
		return OOC_TOOL_ERROR_FEW_PARAMETERS;
		}
		
	settings_set_defaults_I( & settings );
	
	for( i = 1; i < argc; i++ ) {
		if( not g_strcmp0( argv[i], "-h" ) or not g_strcmp0( argv[i], "--help" ) ) {
			print_help( );
			return OOC_TOOL_OK;
			}
			
		else if( not g_strcmp0( argv[i], "-n" ) or not g_strcmp0( argv[i], "--new" ) ) {
			if( ( i+1 == argc ) or ( argv[i+1][0] == '-' ) ) {
				g_printerr( "Missing class name to create\n" );
				return OOC_TOOL_ERROR_FEW_PARAMETERS;
				}
			settings.new_class.original = argv[ ++ i ];
			operation = OPERATION_NEW;
			}
			
		else if( not g_strcmp0( argv[i], "-f" ) or not g_strcmp0( argv[i], "--from" ) ) {
			if( ( i+1 == argc ) or ( argv[i+1][0] == '-' ) ) {
				g_printerr( "Missing template class name\n" );
				return OOC_TOOL_ERROR_FEW_PARAMETERS;
				}
			settings.from_class.original = argv[ ++ i ];
			}
			
		else if( not g_strcmp0( argv[i], "-p" ) or not g_strcmp0( argv[i], "--parent" ) ) {
			if( ( i+1 == argc ) or ( argv[i+1][0] == '-' ) ) {
				g_printerr( "Missing parent class name\n" );
				return OOC_TOOL_ERROR_FEW_PARAMETERS;
				}
			settings.new_parent.original = argv[ ++ i ];
			}
			
		else if( not g_strcmp0( argv[i], "-t" ) or not g_strcmp0( argv[i], "--target" ) ) {
			if( ( i+1 == argc ) or ( argv[i+1][0] == '-' ) ) {
				g_printerr( "Missing target file name\n" );
				return OOC_TOOL_ERROR_FEW_PARAMETERS;
				}
			settings.output_dir  = g_path_get_dirname( argv[ ++ i ] );
			settings.output_file = g_path_get_basename( argv[ i ] );
			}
			
		else if( not g_strcmp0( argv[i], "-s" ) or not g_strcmp0( argv[i], "--source" ) or not g_strcmp0( argv[i], "--template" ) ) {
			if( ( i+1 == argc ) or ( argv[i+1][0] == '-' ) ) {
				g_printerr( "Missing source file name\n" );
				return OOC_TOOL_ERROR_FEW_PARAMETERS;
				}
			++i;
			if( g_path_is_absolute ( argv[ i ] ) )
				settings.input_dir  = g_path_get_dirname( argv[ i ] );
			settings.input_file = g_path_get_basename( argv[ i ] );
			}
			
		else if( g_strcmp0( argv[i], "--testsuite" ) == 0 ) {
			if( argc != 2 ) {
				g_printerr( "--testsuite command has no other parameters!\n" );
				return OOC_TOOL_ERROR_FEW_PARAMETERS;
				}
			operation = OPERATION_TESTSUITE;
			}
			
		else {
			g_printerr( "Unknown parameter: %s\n", argv[i] );
			return OOC_TOOL_ERROR_BAD_PARAMETER;
			}
		}

	if( operation == OPERATION_NONE ) {
		g_printerr( "Operation was not defined\n" );
		return OOC_TOOL_ERROR_FEW_PARAMETERS;
		}	
		
	settings_set_defaults_II( & settings );
	
	return OOC_TOOL_OK;
}

void
replace_string( GString * line, gssize found, gssize len, const char * new_str )
{
	g_string_erase( line, found, len );
	g_string_insert( line, found, new_str ); 
}

void
replace_names( GString * line, struct ClassNameStruct * what, struct ClassNameStruct * with )
{
	gssize 	pos, len;
	char	first_char;
	
	len = strlen( what->original );
	
	if( len == 0 )
		return;
		
	first_char = g_ascii_tolower( what->lowercase[0] );
	
	pos = 0;
	while( line->str[ pos ] ) {
		if( g_ascii_tolower( line->str[ pos ] ) == first_char ) {
			if( strncmp( & line->str[ pos ], what->lowercase, len ) == 0 ) {
				replace_string( line, pos, len, with->lowercase );
				pos += strlen( with->lowercase ) - 1;
				}
			else if( strncmp( & line->str[ pos ], what->uppercase, len ) == 0 ) {
				replace_string( line, pos, len, with->uppercase );
				pos += strlen( with->uppercase ) - 1;
				}
			else if( strncmp( & line->str[ pos ], what->original, len ) == 0 ) {
				replace_string( line, pos, len, with->original );
				pos += strlen( with->original ) - 1;
				}
			}
		++ pos;
		}
}

void
replace_line( GString * line, Settings set )
{
	if( g_strstr_len( line->str, line->len, "#include" ) == NULL
		|| g_strcmp0( set->output_file, set->new_class.lowercase ) == 0 )
		
		replace_names( line, & set->from_class, & set->new_class );
		
	else {
		struct ClassNameStruct filename;
		
		filename.lowercase = set->output_file;
		filename.original  = set->output_file;
		filename.uppercase = set->output_file;

		replace_names( line, & set->from_class, & filename );
		}	
	
	if( set->process_parent )
		replace_names( line, & set->from_parent, & set->new_parent );
}


int
replace_file( GIOChannel* input, GIOChannel* output, Settings set )
{
	GIOStatus  	status;
	GString	*	line;
	GError *	error = NULL;
		
	line = g_string_sized_new( 256 );
	
	while( TRUE ) {
		
		status = g_io_channel_read_line_string ( input, line, NULL, & error );

		if( status == G_IO_STATUS_EOF )
			break;
			
		if( error != NULL ) {
			g_printerr( "I/O error while reading file: %s\n", error->message );
			break;
			}

		replace_line( line, set );

		status = g_io_channel_write_chars( output, line->str, line->len, NULL, & error);
		
		if( error != NULL ) {
			g_printerr( "I/O error while writing file: %s\n", error->message );
			break;
			}

		};
		
	return ( status == G_IO_STATUS_ERROR ) ? OOC_TOOL_ERROR_FILE_IO_PROBLEM : OOC_TOOL_OK;
}

int
replace_templates( Settings set )
{
	char * file_processing;
	
	char * input_filename = NULL;
	char * output_filename = NULL;
	
	GIOChannel * input = NULL;
	GIOChannel * output = NULL;
	
	GError *	error = NULL;
	GString	*	str;
	
	str = g_string_sized_new( 128 );
	
	/* Converting the implementation file */
	{
		int result;
		
		g_string_assign( str, set->input_file );
		g_string_append( str, ".c" );
		input_filename = g_build_filename( set->input_dir, str->str, NULL );
		file_processing = input_filename;
		input = g_io_channel_new_file( input_filename, "r", & error );
		if( not input ) goto Error;
		
		if( ( g_strcmp0( set->input_file, "interface" ) == 0 ) && ( g_strcmp0( set->output_file, set->new_class.lowercase ) == 0 ) )
			g_string_assign( str, "interfaces" );
		else
			g_string_assign( str, set->output_file );
		g_string_append( str, ".c" );
		output_filename = g_build_filename( set->output_dir, str->str, NULL );
		file_processing = output_filename;
		output = g_io_channel_new_file( output_filename, "a", & error );
		if( not output ) goto Error;
		
		result = replace_file( input, output, set );
	
		g_io_channel_shutdown( input, TRUE, &error );
		g_io_channel_unref( input );
		input = NULL;
		
		g_io_channel_shutdown( output, TRUE, &error );
		g_io_channel_unref( output );
		output = NULL;
	
		g_free( input_filename );
		input_filename = NULL;
		g_free( output_filename );
		output_filename = NULL;
		
		if( result != OOC_TOOL_OK ) goto Error;
	}
	
	/* Converting the header file */
	{
		int result = OOC_TOOL_OK;
		
		g_clear_error( &error );
		
		g_string_assign( str, set->input_file );
		g_string_append( str, ".h" );
		input_filename = g_build_filename( set->input_dir, str->str, NULL );
		file_processing = input_filename;
		input = g_io_channel_new_file( input_filename, "r", & error );
		if( input ) {
			
			g_string_assign( str, set->output_file );
			g_string_append( str, ".h" );
			output_filename = g_build_filename( set->output_dir, str->str, NULL );
			file_processing = output_filename;
			output = g_io_channel_new_file( output_filename, "a", & error );
			if( not output ) goto Error;
			
			result = replace_file( input, output, set );
		
			g_io_channel_shutdown( input, TRUE, &error );
			g_io_channel_unref( input );
			input = NULL;
			
			g_io_channel_shutdown( output, TRUE, &error );
			g_io_channel_unref( output );
			output = NULL;
		
			g_free( output_filename );
			output_filename = NULL;
			}
		g_free( input_filename );
		input_filename = NULL;
		
		if( result != OOC_TOOL_OK ) goto Error;
	}
	
	/* Converting the implementation header file */
	{
		int result = OOC_TOOL_OK;
		
		g_clear_error( &error );
		
		g_string_assign( str, set->input_file );
		g_string_append( str, ".h" );
		input_filename = g_build_filename( set->input_dir, set->subfolder, str->str, NULL );
		file_processing = input_filename;
		input = g_io_channel_new_file( input_filename, "r", & error );
		if( input ) {
		
			g_string_assign( str, set->output_file );
			g_string_append( str, ".h" );
			
			output_filename = g_build_filename( set->output_dir, set->subfolder, NULL );
			g_mkdir_with_parents ( output_filename, 0777 );
			g_free( output_filename );
			
			output_filename = g_build_filename( set->output_dir, set->subfolder, str->str, NULL );
			file_processing = output_filename;
			output = g_io_channel_new_file( output_filename, "a", & error );
			if( not output ) goto Error;
			
			result = replace_file( input, output, set );
		
			g_io_channel_shutdown( input, TRUE, &error );
			g_io_channel_unref( input );
			input = NULL;
			
			g_io_channel_shutdown( output, TRUE, &error );
			g_io_channel_unref( output );
			output = NULL;
		
			g_free( output_filename );
			output_filename = NULL;
		}
		g_free( input_filename );
		input_filename = NULL;
		
		if( result != OOC_TOOL_OK ) goto Error;
	}

	g_string_free ( str, TRUE );

	return OOC_TOOL_OK;
		
	Error:
		g_printerr( "I/O error: %s : %s\n", file_processing, error->message );
		if( input ) {
			g_io_channel_shutdown( input, TRUE, NULL );
			g_io_channel_unref( input );
			}
		if( output ) {
			g_io_channel_shutdown( output, TRUE, NULL );
			g_io_channel_unref( output );
			}
		if( input_filename )
			g_free( input_filename );
		if( output_filename )
			g_free( output_filename );
		g_string_free ( str, TRUE );
		return OOC_TOOL_ERROR_CANT_OPEN_FILE;
}

static
void
create_testsuite( Settings set )
{
	char * command;
	
#ifndef WIN32
	command = g_strdup_printf( "cp -u %s/unit/* .",  set->input_dir );
#else
	command = g_strdup_printf( "copy %s\\unit\\*.*",  set->input_dir );
#endif
	
	if( command ) {
		system( command );
		g_free( command );
		}
}

int
main( int argc, char * argv[] )
{
	int error = OOC_TOOL_OK;
	
#if defined( WIN32 ) && defined( NDEBUG )
	template_dir = win_template_dir( argv[0] );
#endif

	error = get_args( argc, argv );
	
	if( operation == OPERATION_NEW ) {
		
		replace_templates( & settings );
		
		}
	else if( operation == OPERATION_TESTSUITE ) {
		
		create_testsuite( & settings );
		
		};
		
	settings_release( & settings );

#if defined( WIN32 ) && defined( NDEBUG )
	g_free( template_dir );
#endif
	
	return error;
}
