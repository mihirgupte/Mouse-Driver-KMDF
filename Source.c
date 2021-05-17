#include <ntddk.h>
#pragma warning( disable : 4244 )
#pragma warning( disable : 4242 )
typedef struct {
    PDEVICE_OBJECT mouse_btn;
}DEVICE_EXTENSION, * PDEVICE_EXTENSION;

typedef struct _MOUSE_INPUT_DATA {
    USHORT UnitId;
    USHORT Flags;
    union {
        ULONG Buttons;
        struct {
            USHORT ButtonFlags;
            USHORT ButtonData;
        }b1;
    }b2;
    ULONG  RawButtons;
    LONG   LastX;
    LONG   LastY;
    ULONG  ExtraInformation;
} MOUSE_INPUT_DATA, * PMOUSE_INPUT_DATA;

PDEVICE_OBJECT myMouse = NULL;
ULONG pendingbtn = 0;

void DriverUnload(PDRIVER_OBJECT DriverObject) {
    LARGE_INTEGER interval = { 0 };
    PDEVICE_OBJECT DeviceObject = DriverObject->DeviceObject;
    interval.QuadPart = -10 * 1000 * 1000;
    IoDetachDevice(((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->mouse_btn);
    while (pendingbtn) {
        KeDelayExecutionThread(KernelMode, FALSE, &interval);
    }
    IoDeleteDevice(myMouse);
    KdPrint(("Unload Driver \n"));
}

NTSTATUS DispatchPass(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    IoCopyCurrentIrpStackLocationToNext(Irp);
    return IoCallDriver(((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->mouse_btn, Irp);
}

NTSTATUS ReadComplete(PDEVICE_OBJECT DeviceObject, PIRP Irp, PVOID Context) {
    DeviceObject;
    Context;
    PMOUSE_INPUT_DATA buttons = (PMOUSE_INPUT_DATA)Irp->AssociatedIrp.SystemBuffer;
    int structnum = Irp->IoStatus.Information / sizeof(MOUSE_INPUT_DATA);
    int i;

    if (Irp->IoStatus.Status == STATUS_SUCCESS) {
        for (i = 0;i < structnum;i++) {
            KdPrint(("the button state is %x \n", buttons->b2));
        }
    }

    if (Irp->PendingReturned) {
        IoMarkIrpPending(Irp);
    }

    pendingbtn--;
    return Irp->IoStatus.Status;
}

NTSTATUS DispatchRead(PDEVICE_OBJECT DeviceObject, PIRP Irp) {
    IoCopyCurrentIrpStackLocationToNext(Irp);
    IoSetCompletionRoutine(Irp, ReadComplete, NULL, TRUE, TRUE, TRUE);
    pendingbtn++;
    return IoCallDriver(((PDEVICE_EXTENSION)DeviceObject->DeviceExtension)->mouse_btn, Irp);
}

NTSTATUS MyAttachDevice(PDRIVER_OBJECT DriverObject) {
    NTSTATUS status;
    UNICODE_STRING TargetDevice = RTL_CONSTANT_STRING(L"\\Device\\PointerClass0");
    status = IoCreateDevice(DriverObject, sizeof(DEVICE_EXTENSION), NULL, FILE_DEVICE_MOUSE, 0, FALSE, &myMouse);
    if (!NT_SUCCESS(status)) {
        return status;
    }
    myMouse->Flags |= DO_BUFFERED_IO;
    myMouse->Flags &= ~DO_DEVICE_INITIALIZING;

    RtlZeroMemory(myMouse->DeviceExtension, sizeof(DEVICE_EXTENSION));
    status = IoAttachDevice(myMouse, &TargetDevice, &((PDEVICE_EXTENSION)myMouse->DeviceExtension)->mouse_btn);
    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(myMouse);
        return status;
    }
    return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    RegistryPath;
    NTSTATUS status;
    int i;
    DriverObject->DriverUnload = DriverUnload;

    for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++) {
        DriverObject->MajorFunction[i] = DispatchPass;
    }

    DriverObject->MajorFunction[IRP_MJ_READ] = DispatchRead;
    status = MyAttachDevice(DriverObject);

    if (!NT_SUCCESS(status)) {
        KdPrint(("Attaching Failed \r\n"));
    }
    else {
        KdPrint(("attaching succeded \r\n"));
    }
    return status;
}