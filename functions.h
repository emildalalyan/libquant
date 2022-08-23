#pragma once

/* Enumeration of possible statuses, that're returning by functions. */
enum func_status
{
   /* Function executed successfully. */
   FUNC_OK = 0,

   /* Function couldn't read or write some part(s) of file. */
   FUNC_IO_ERROR = 1,

   /* Signatures weren't right, so file's in wrong format or it's corrupted. */
   FUNC_SIGNATURE_FAILURE = 2,

   /* Function does not support this file or format. */
   FUNC_UNSUPPORTED = 3,

   /* Argument provided to the function is invalid. */
   FUNC_INVALID_ARG = 4,

   /* Internal error occurred during function execution. */
   FUNC_INTERNAL_ERROR = 5,

   /* Allocation of memory chunk has been failed. */
   FUNC_MEMALLOC_FAILED = 6
};