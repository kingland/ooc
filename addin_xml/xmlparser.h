
#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdio.h>

DeclareClass( XmlParser, XmlBase );

XmlParser	xmlparser_new_file( FILE * );
XmlParser	xmlparser_new_str( char * str );

Object 		xmlparser_parse( XmlParser self );

void		_xmlparser_set_type( XmlParser, Class );
void		_xmlparser_set_root( XmlParser, Object );

#endif  /* XMLPARSER_H */
