/**
 * @file ${FILE}
 * @brief 
 * @license ${LICENSE}
 * Copyright (c) 10/16/15, Steven
 * 
 **/
#ifndef DANGERENGINE_INTRUSIVELIST_H
#define DANGERENGINE_INTRUSIVELIST_H

#define SLS_LIST_FIELDNAME _ilist_field

/**
 * @brief adds intrusive linked list field to a structure
 */
#define SLS_INTRUSIVE_FIELDS(typename, field_name) struct {\
  typename *prev; \
  typename  *next; \
} field_name


#endif //DANGERENGINE_INTRUSIVELIST_H
