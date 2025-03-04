/** @file
  SM3 Digest Wrapper Implementation over OpenSSL.

Copyright (c) 2009 - 2016, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "InternalCryptLib.h"
#include <openssl/sm3.h>

/**
  Retrieves the size, in bytes, of the context buffer required for SM3 hash operations.

  @return  The size, in bytes, of the context buffer required for SM3 hash operations.

**/
UINTN
EFIAPI
Sm3GetContextSize (
  VOID
  )
{
  //
  // Retrieves OpenSSL SM3 Context Size
  //
  return (UINTN) (sizeof (SM3_CTX));
}

/**
  Initializes user-supplied memory pointed by Sm3Context as SM3 hash context for
  subsequent use.

  If Sm3Context is NULL, then return FALSE.

  @param[out]  Sm3Context  Pointer to SM3 context being initialized.

  @retval TRUE   SM3 context initialization succeeded.
  @retval FALSE  SM3 context initialization failed.

**/
BOOLEAN
EFIAPI
Sm3Init (
  OUT  VOID  *Sm3Context
  )
{
  //
  // Check input parameters.
  //
  if (Sm3Context == NULL) {
    return FALSE;
  }

  //
  // OpenSSL SM3 Context Initialization
  //
  return (BOOLEAN) (sm3_init ((SM3_CTX *) Sm3Context));
}

/**
  Makes a copy of an existing SM3 context.

  If Sm3Context is NULL, then return FALSE.
  If NewSm3Context is NULL, then return FALSE.

  @param[in]  Sm3Context     Pointer to SM3 context being copied.
  @param[out] NewSm3Context  Pointer to new SM3 context.

  @retval TRUE   SM3 context copy succeeded.
  @retval FALSE  SM3 context copy failed.

**/
BOOLEAN
EFIAPI
Sm3Duplicate (
  IN   CONST VOID  *Sm3Context,
  OUT  VOID        *NewSm3Context
  )
{
  //
  // Check input parameters.
  //
  if (Sm3Context == NULL || NewSm3Context == NULL) {
    return FALSE;
  }

  CopyMem (NewSm3Context, (void *)Sm3Context, sizeof (SM3_CTX));

  return TRUE;
}

/**
  Digests the input data and updates SM3 context.

  This function performs SM3 digest on a data buffer of the specified size.
  It can be called multiple times to compute the digest of long or discontinuous data streams.
  SM3 context should be already correctly initialized by Sm3Init(), and should not be finalized
  by Sm3Final(). Behavior with invalid context is undefined.

  If Sm3Context is NULL, then return FALSE.

  @param[in, out]  Sm3Context  Pointer to the SM3 context.
  @param[in]       Data           Pointer to the buffer containing the data to be hashed.
  @param[in]       DataSize       Size of Data buffer in bytes.

  @retval TRUE   SM3 data digest succeeded.
  @retval FALSE  SM3 data digest failed.

**/
BOOLEAN
EFIAPI
Sm3Update (
  IN OUT  VOID        *Sm3Context,
  IN      CONST VOID  *Data,
  IN      UINTN       DataSize
  )
{
  //
  // Check input parameters.
  //
  if (Sm3Context == NULL) {
    return FALSE;
  }

  //
  // Check invalid parameters, in case that only DataLength was checked in OpenSSL
  //
  if (Data == NULL && DataSize != 0) {
    return FALSE;
  }

  //
  // OpenSSL SM3 Hash Update
  //
  return (BOOLEAN) (sm3_update ((SM3_CTX *) Sm3Context, Data, DataSize));
}

/**
  Completes computation of the SM3 digest value.

  This function completes SM3 hash computation and retrieves the digest value into
  the specified memory. After this function has been called, the SM3 context cannot
  be used again.
  SM3 context should be already correctly initialized by Sm3Init(), and should not be
  finalized by Sm3Final(). Behavior with invalid SM3 context is undefined.

  If Sm3Context is NULL, then return FALSE.
  If HashValue is NULL, then return FALSE.

  @param[in, out]  Sm3Context  Pointer to the SM3 context.
  @param[out]      HashValue      Pointer to a buffer that receives the SM3 digest
                                  value (32 bytes).

  @retval TRUE   SM3 digest computation succeeded.
  @retval FALSE  SM3 digest computation failed.

**/
BOOLEAN
EFIAPI
Sm3Final (
  IN OUT  VOID   *Sm3Context,
  OUT     UINT8  *HashValue
  )
{
  //
  // Check input parameters.
  //
  if (Sm3Context == NULL || HashValue == NULL) {
    return FALSE;
  }

  //
  // OpenSSL SM3 Hash Finalization
  //
  return (BOOLEAN) (sm3_final (HashValue, (SM3_CTX *) Sm3Context));
}

/**
  Computes the SM3 message digest of a input data buffer.

  This function performs the SM3 message digest of a given data buffer, and places
  the digest value into the specified memory.

  If this interface is not supported, then return FALSE.

  @param[in]   Data        Pointer to the buffer containing the data to be hashed.
  @param[in]   DataSize    Size of Data buffer in bytes.
  @param[out]  HashValue   Pointer to a buffer that receives the SM3 digest
                           value (32 bytes).

  @retval TRUE   SM3 digest computation succeeded.
  @retval FALSE  SM3 digest computation failed.
  @retval FALSE  This interface is not supported.

**/
BOOLEAN
EFIAPI
Sm3HashAll (
  IN   CONST VOID  *Data,
  IN   UINTN       DataSize,
  OUT  UINT8       *HashValue
  )
{
  //
  // Check input parameters.
  //
  if (HashValue == NULL) {
    return FALSE;
  }
  if (Data == NULL && DataSize != 0) {
    return FALSE;
  }

  //
  // OpenSSL SM3 Hash Computation.
  //

  SM3_CTX c;
  sm3_init(&c);
  sm3_update(&c, Data, DataSize);
  sm3_final(HashValue, &c);

  return TRUE;
}
