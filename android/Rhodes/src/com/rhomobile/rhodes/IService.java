/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: C:\\Users\\Ilan\\Android\\ServiceExample\\src\\org\\android\\service\\IService.aidl
 */
package com.rhomobile.rhodes;

import android.os.IBinder;


public interface IService extends android.os.IInterface {
	/** Local-side IPC implementation stub class. */
	public static abstract class Stub extends android.os.Binder implements
			com.rhomobile.rhodes.IService {
		private static final java.lang.String DESCRIPTOR = "com.rhomobile.rhodes.IService";

		/** Construct the stub at attach it to the interface. */
		public Stub() {
			this.attachInterface(this, DESCRIPTOR);
		}

		/**
		 * Cast an IBinder object into an IService interface, generating a proxy
		 * if needed.
		 */
		public static com.rhomobile.rhodes.IService asInterface(
				android.os.IBinder obj) {
			if ((obj == null)) {
				return null;
			}
			com.rhomobile.rhodes.IService in = (com.rhomobile.rhodes.IService) obj
					.queryLocalInterface(DESCRIPTOR);
			if ((in != null)) {
				return in;
			}
			return new com.rhomobile.rhodes.IService.Stub.Proxy(obj);
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
			case TRANSACTION_registerCallback: {
				data.enforceInterface(DESCRIPTOR);
				com.rhomobile.rhodes.ICallback _arg0;
				_arg0 = com.rhomobile.rhodes.ICallback.Stub.asInterface(data
						.readStrongBinder());
				this.registerCallback(_arg0);
				reply.writeNoException();
				return true;
			}
			case TRANSACTION_unregisterCallback: {
				data.enforceInterface(DESCRIPTOR);
				com.rhomobile.rhodes.ICallback _arg0;
				_arg0 = com.rhomobile.rhodes.ICallback.Stub.asInterface(data
						.readStrongBinder());
				this.unregisterCallback(_arg0);
				reply.writeNoException();
				return true;
			}
			case TRANSACTION_setValue: {
				data.enforceInterface(DESCRIPTOR);
				int _arg0;
				_arg0 = data.readInt();
				this.setValue(_arg0);
				reply.writeNoException();
				return true;
			}
			case TRANSACTION_getValue: {
				data.enforceInterface(DESCRIPTOR);
				int _result = this.getValue();
				reply.writeNoException();
				reply.writeInt(_result);
				return true;
			}
			}
			return super.onTransact(code, data, reply, flags);
		}

		private static class Proxy implements com.rhomobile.rhodes.IService {
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

			public void registerCallback(com.rhomobile.rhodes.ICallback cb)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeStrongBinder((((cb != null)) ? (cb.asBinder())
							: (null)));
					mRemote.transact(Stub.TRANSACTION_registerCallback, _data,
							_reply, 0);
					_reply.readException();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
			}

			public void unregisterCallback(com.rhomobile.rhodes.ICallback cb)
					throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeStrongBinder((((cb != null)) ? (cb.asBinder())
							: (null)));
					mRemote.transact(Stub.TRANSACTION_unregisterCallback,
							_data, _reply, 0);
					_reply.readException();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
			}

			public void setValue(int value) throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					_data.writeInt(value);
					mRemote.transact(Stub.TRANSACTION_setValue, _data, _reply,
							0);
					_reply.readException();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
			}

			public int getValue() throws android.os.RemoteException {
				android.os.Parcel _data = android.os.Parcel.obtain();
				android.os.Parcel _reply = android.os.Parcel.obtain();
				int _result;
				try {
					_data.writeInterfaceToken(DESCRIPTOR);
					mRemote.transact(Stub.TRANSACTION_getValue, _data, _reply,
							0);
					_reply.readException();
					_result = _reply.readInt();
				} finally {
					_reply.recycle();
					_data.recycle();
				}
				return _result;
			}
		}

		static final int TRANSACTION_registerCallback = (IBinder.FIRST_CALL_TRANSACTION + 0);
		static final int TRANSACTION_unregisterCallback = (IBinder.FIRST_CALL_TRANSACTION + 1);
		static final int TRANSACTION_setValue = (IBinder.FIRST_CALL_TRANSACTION + 2);
		static final int TRANSACTION_getValue = (IBinder.FIRST_CALL_TRANSACTION + 3);
	}

	public void registerCallback(com.rhomobile.rhodes.ICallback cb)
			throws android.os.RemoteException;

	public void unregisterCallback(com.rhomobile.rhodes.ICallback cb)
			throws android.os.RemoteException;

	public void setValue(int value) throws android.os.RemoteException;

	public int getValue() throws android.os.RemoteException;
}
