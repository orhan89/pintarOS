/*! @file
	\brief TEA declarations.

	Documentation for TEA is available at
	http://www.cl.cam.ac.uk/ftp/users/djw3/tea.ps.

	$Id: tea.h,v 1.5 2002/12/22 15:42:55 m Exp $
*/

#ifndef TEA_H
#define TEA_H

#include "types.h"

#define iu32    uint32_t
#define iu16    uint16_t
#define iu8     uint8_t

#define TEA_KEY_LEN		16	//!< TEA key size.
#define TEA_BLOCK_LEN	        8	//!< TEA block length.

#define DELTA	0x9E3779B9	//!< Magic value. (Golden number * 2^31)
#define ROUNDS	32			//!< Number of rounds.

//! Byte swap single short.
#define swap_us(us) (((us&0x00FF)<<8)|((us&0xFF00)>>8))
//! Byte swap single long.
#define swap_ul(ul) (((ul&0x000000FF)<<24)|((ul&0x0000FF00)<<8)|((ul&0x00FF0000)>>8)|((ul&0xFF000000)>>24))

/*! \brief Byte swap multiple shorts.

	\param us Pointer to an array of shorts.
	\param num Number of shorts to process.
*/
void hton_us( uint16_t *us, uint8_t num );
/*! \brief Byte swap multiple longs.

	\param ul Pointer to an array of longs.
	\param num Number of longs to process.
*/
void hton_ul( uint32_t *ul, uint8_t num );

#ifndef ENDIAN_LITTLE
#define hton_us(us)
#define hton_ul(ul)
#endif

/*! Return minimum value */
#define min(a,b) ((a)<(b)?(a):(b))

/*! Return maximum value */
#define max(a,b) ((a)>(b)?(a):(b))

/*! \brief TEA encryption function.

	This function encrypts <EM>v</EM> with <EM>k</EM> and returns the
	encrypted data in <EM>v</EM>.

	\param v Array of two long values containing the data block.
	\param k Array of four long values containing the key.
*/
void tea_enc( uint32_t *v, uint32_t *k );

/*! \brief TEA decryption function.

	This function decrypts <EM>v</EM> with <EM>k</EM> and returns the
	decrypted data in <EM>v</EM>.

	\param v Array of two long values containing the data block.
	\param k Array of four long values containing the key.
*/
void tea_dec( uint32_t *v, uint32_t *k );

#endif 
