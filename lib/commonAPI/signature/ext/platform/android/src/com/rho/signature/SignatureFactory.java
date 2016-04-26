package com.rho.signature;

import com.rho.signature.ISignature;
import com.rho.signature.ISignatureFactory;
import com.rho.signature.ISignatureSingleton;

public class SignatureFactory implements ISignatureFactory
{
	private SignatureSingleton mSingleton = null;// new SignatureSingleton();

	@Override
	public ISignatureSingleton getApiSingleton()
	{
		if (mSingleton == null)
		{
			mSingleton = new SignatureSingleton();
		}
		return mSingleton;
	}

	@Override
	public ISignature getApiObject(String id)
	{
		return null;
	}
}
