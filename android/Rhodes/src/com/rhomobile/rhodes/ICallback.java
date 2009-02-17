/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: C:\\Users\\Ilan\\Android\\ServiceExample\\src\\org\\android\\service\\ICallback.aidl
 */
package com.rhomobile.rhodes;

import android.os.IBinder;

public interface ICallback extends android.os.IInterface {
	/** Local-side IPC implementation stub class. */
	public static abstract class Stub extends android.os.Binder implements
			com.rhomobile.rhodes.ICallback {
		private static final java.lang.String DESCRIPTOR = "org.android.service.ICallback";

		/** Construct the stub at attach it to the interface. */
		public Stub() {
			this.attachInterface(this, DESCRIPTOR);
		}

		/**
		 * Cast an IBinder object into an ICallback interface, generating a
		 * proxy if needed.
		 */
		public static com.rhomobile.rhodes.ICallback asInterface(
				android.os.IBinder obj) {
			if ((obj == null)) {
				return null;
			}
			com.rhomobile.rhodes.ICallback in = (com.rhomobile.rhodes.ICallback) obj
					.queryLocalInterface(DESCRIPTOR);
			if ((in != null)) {
				return in;
			}
			return new com.rhomobile.rhodes.ICallback.Stub.Proxy(obj);
		}

		public android.os.IBinder asBinder() {
			return this;
		}

		public boolean onTransact(int code, android.os.Parcel data,
				android.os.Parcel reply, int flags)
				throws android.os.RemoteException {
			switch (code) {
			case INTERFACE_TRANSACTION: {
				reply.writeString(DESCRIPTOR);
				return true;
			}
			case TRANSACTION_valueChanged: {
				data.enforceInterface(DESCRIPTOR);
				int _arg0;
				_arg0 = data.readInt();
				this.valueChanged(_arg0);
				reply.writeNoException();
				return true;
			}
			}
			return super.onTransact(code, data, reply, flags);
		}

		private static class Proxy implements com.rhomobile.rhodes.ICallback {
			private android.os.IBinder mRemote;

			Proxy(android.os.IBinder remote) {
				mRemote = remote;
			}

			public android.os.IBinder asBinder() {
				return mRemote;
			}

			public java.lang.String getInterfaceDescriptor() {
				return DESCRIPTOR;
			}

			public void valueChanged(int value)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeInt(value);
					mRemote.transact(Stub.TRANSACTION_valueChanged, _data,
							_reply, 0);
					_reply.readException();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
			}
		}

		static final int TRANSACTION_valueChanged = (IBinder.FIRST_CALL_TRANSACTION + 0);
	}

	public void valueChanged(int value) throws android.os.RemoteException;
}
