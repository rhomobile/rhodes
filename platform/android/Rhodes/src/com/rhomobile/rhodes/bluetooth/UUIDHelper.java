package com.rhomobile.rhodes.bluetooth;


import java.util.UUID;

public class UUIDHelper {

	/** SDP 0x0001 16-bit */
	public static final UUID SDP_PROTOCOL_UUID = fromUUID16(0x0001);

	/** RFCOMM 0x0003 16-bit */
	public static final UUID RFCOMM_PROTOCOL_UUID = fromUUID16(0x0003);

	/** OBEX 0x0008 16-bit */
	public static final UUID OBEX_PROTOCOL_UUID = fromUUID16(0x0008);

	/** HTTP 0x000C 16-bit */
	public static final UUID HTTP_PROTOCOL_UUID = fromUUID16(0x000C);

	/** L2CAP 0x0100 16-bit */
	public static final UUID L2CAP_PROTOCOL_UUID = fromUUID16(0x0100);

	/** BNEP 0x000F 16-bit */
	public static final UUID BNEP_PROTOCOL_UUID = fromUUID16(0x000F);

	/** Serial Port 0x1101 16-bit */
	public static final UUID SERIAL_PORT_PROTOCOL_UUID = fromUUID16(0x1101);

	/** OBEX Object Push Profile 0x1105 16-bit */
	public static final UUID OBEX_OBJECT_PUSH_PROTOCOL_UUID = fromUUID16(0x1105);

	/** OBEX File Transfer Profile 0x1106 16-bit */
	public static final UUID OBEX_FILE_TRANSFER_PROTOCOL_UUID = fromUUID16(0x1106);

	/** Personal Area Networking User 0x1115 16-bit */
	public static final UUID PERSONAL_AREA_NETWORKING_PROTOCOL_UUID = fromUUID16(0x1115);

	private static final String UUID_BASE = "0000-1000-8000-00805F9B34FB";

	private static final int MASK_UUID16 = 0x0000FFFF;

	private static final int MASK_UUID128 = 0x0000FFFF;

	public static final UUID fromUUID16(int uuid16) {

		String s = uuidStringFromUuid16(uuid16);
		UUID uuid = UUID.fromString(s);

		return uuid;
	}

	public static final int toUUID16(UUID uuid128) {

		long bits = uuid128.getMostSignificantBits();

		return (int) ((bits >> 32) & MASK_UUID128);
	}

	private static final String uuidStringFromUuid16(int uuid16) {

		StringBuilder b = new StringBuilder();
		String hex = Integer.toHexString(uuid16 & MASK_UUID16);
		b.append("00000000".substring(hex.length()));
		b.append(hex);
		b.append('-');
		b.append(UUID_BASE);

		return b.toString();
	}

}
