#pragma once
#define ATB_EID_BUFFER_WIDTH 17

#define MAX_UNALLOCATED_EIDS 5
#define MAX_BAILS 200
#define CIB_MAX_PRESENCE 5

typedef char atb_eid_t[ATB_EID_BUFFER_WIDTH];

typedef struct atb_eid_data {
	DWORD count; //if count == 0 then the structure is not in use
	atb_eid_t eid;
	DWORD first_ticks;
	DWORD offset_sum;
	DWORD last_ticks;
	ULONGLONG first_read_time;
	ULONGLONG last_read_time;
	ULONGLONG offset_sum_read_time;
} atb_eid_data_t;

typedef struct atb_bail_data {
	BYTE cow_type;
	atb_eid_t eid;
	WORD sequence_id;
	DWORD count; //if count == 0 then there are no eids
	DWORD first_ticks;
	DWORD offset_sum;
	DWORD last_ticks;
} atb_bail_data_t;

typedef struct atb_state {
	atb_eid_data_t unallocated[MAX_UNALLOCATED_EIDS];
	BYTE next_bail_number;
	BYTE bail_plate;

	atb_bail_data_t bail_allocations[MAX_BAILS];

	BYTE input_state;

	DWORD eid_count;

	atb_eid_t last_eid;
	DWORD last_eid_time;

	BYTE cib_state;
	int cib_polls_remaining;
	int cib_poll_ttl;
	DWORD cib_poll_start;
	BYTE cib_presence[CIB_MAX_PRESENCE];

	DWORD fbm_time[3];
	BYTE fbm_count;

	DWORD average_eid_count;		//stores the average number of eid reads per processed eid/bail
	DWORD number_eids_processed;	//stores the number of processed bails for use as a weight in the average calculation

	DWORD last_keepalive_time;
	BYTE leading_dirty_count;
	BYTE trailing_dirty_count;

	DWORD time_of_last_line_or_error;
	DWORD eid_error_count;
	BYTE error_flags;
	WORD reallocations;
	WORD rbm_count;

	BYTE platform_stopped;
	DWORD platform_stopped_timer_start;
	BYTE  bail_movement;
} atb_state_t;