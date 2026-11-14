#ifndef COMMAND_CODES_H
#define COMMAND_CODES_H

/* =========================================
 * Network 
 * ========================================= */
#define NetworkSerialNumberGet             0x01
#define NetworkSourceAddressSet            0x02
#define NetworkNextTokenRingAddressSet     0x03
#define NetworkTokenRingAddressForward     0x04
#define NetworkReset                       0x05

/* =========================================
 * Application Layer
 * ========================================= */
// Client
#define ClientAcquisitionConfigSet         0x06
#define ClientAcquisitionConfigGet         0x07
#define ClientChannelConfigSet             0x09
#define ClientChannelConfigGet             0x0A
#define ClientAnalogOutConfigSet           0x0E
#define ClientAnalogOutConfigGet           0x0F
#define ClientLampStateSet                 0x15
#define ClientLampStateGet                 0x16
#define ClientGageFactorConfigSet          0x17
#define ClientGageFactorConfigGet          0x18
#define ClientSystemConfigSet              0x1B
#define ClientSystemConfigGet              0x1C
#define ClientAnalogOutStateSet            0x22
#define ClientAnalogOutStateGet            0x23
#define ClientUnitConfigSet                0x2A
#define ClientChannelOffsetConfigSet       0x2B

// Information
#define InformationAcquisitionSystemGet    0x08
#define InformationAcquisitionChannelGet   0x0B
#define InformationAnalogOutGet            0x10

/* =========================================
 * Application Layer (Suite)
 * ========================================= */
// System
#define SystemResetToDefault               0x26
#define SystemReset                        0x28
#define SystemDateTimeGet                  0x00
#define SystemDateTimeSet                  0x01
#define SystemStatisticsGet_X              0x1D
#define SystemStatisticsReset              0x1E

// Firmware
#define FirmwareInfoGet                    0x27

// Acquisition
#define AcquisitionSampleGet               0x29
#define AcquisitionSampleAllChannelGet     0x4F
#define AcquisitionSensorDiagnosticStart   0x42
#define AcquisitionSensorDiagnosticGet     0x43
#define AcquisitionStart                   0x44
#define AcquisitionGet                     0x45
#define AcquisitionStop                    0x46
#define AcquisitionStartTrigger            0x4B
#define AcquisitionFunction                0x4E

#endif // COMMAND_CODES_H
