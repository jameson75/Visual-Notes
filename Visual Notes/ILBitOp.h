
class CILBitOp
{
public:

	static void TurnOn( DWORD *pdwTarget, DWORD dwTargetBits )
	{ *pdwTarget |= dwTargetBits; }

	static void TurnOff( DWORD *pdwTarget, DWORD dwTargetBits )
	{ *pdwTarget^= ( *pdwTarget & dwTargetBits ); }

	static void Toggle( DWORD *pdwTarget, DWORD dwTargetBits )
	{ *pdwTarget^= dwTargetBits; }

	static BOOL IsOn( DWORD dwTarget, DWORD dwTargetBits, BOOL bAll )
	{ 
		if( bAll )//return true if ALL the specified target bits are on.
			return ( (dwTarget & dwTargetBits) == dwTargetBits );
		else //return true if ANY of the specified target bits are on.
			return ( (dwTarget & dwTargetBits) );
	}

	static BOOL IsOff( DWORD dwTarget, DWORD dwTargetBits, BOOL bAll )
	{
		if( bAll ) //return true if ALL the specified target bits are off.
			return !( ( dwTarget & dwTargetBits ) );
		else //return true if ANY of the specified target bits are on.
			return ( ( dwTarget | dwTargetBits ) != dwTarget );
	}
};

		
