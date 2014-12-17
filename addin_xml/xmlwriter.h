
#ifndef XMLWRITER_H
#define XMLWRITER_H

DeclareClass( XmlWriter, XmlBase );

XmlWriter	xmlwriter_new_file( FILE * );
/* XmlWriter	xmlwriter_new_buffer( char *, size_t, handler ); */
void		xmlwriter_write( XmlWriter, Object );

void		xml_set_indent( XmlWriter, int );
int			xml_get_indent( XmlWriter );

void		xml_write_begin_element( XmlWriter, const char * name );
void		xml_write_end_element( XmlWriter );
void		xml_write_attribute( XmlWriter, const char * name, const char * value );
void		xml_write_text( XmlWriter, const char * text );
void		xml_write_element_text( XmlWriter, const char * name, const char * text );
void		xml_write_begin_comment( XmlWriter );
void		xml_write_end_comment( XmlWriter );
void		xml_write_comment( XmlWriter, const char * );

#endif  /* XMLWRITER_H */
