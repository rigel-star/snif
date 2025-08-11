export default interface Packet {
    name: string;
    payload?: Packet | Uint8Array | undefined;

    toJSON(): object;
}