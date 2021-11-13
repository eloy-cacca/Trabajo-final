# -------- Module imports ------- #
import logging as log
from apscheduler.schedulers.background import BackgroundScheduler
from apscheduler.events import EVENT_JOB_EXECUTED, EVENT_JOB_ERROR
from datetime import datetime
import uuid


class Scheduler():
    """
    Every periodic interval, the VFP should:
    - Ask for telemetry
    - Update VFP and show Connnected
    - Send other requests
    - If timeout: show Disconnected
    """
    def __init__(self, backend, backend_api):

        # Initialize scheduler
        self.sched = BackgroundScheduler()
        self.sched.start()
        self.sched.add_listener(self._queue_handler, EVENT_JOB_EXECUTED | EVENT_JOB_ERROR)

        # Add periodic job paused
        self.periodic_job = self.sched.add_job(self.periodic_vfp, "interval", seconds=2, coalesce=True, id="periodic", max_instances=1)
        self.periodic_job.pause()

        # Initialize queues
        self.queue = []
        self.queue_next = []

        # Some variables
        self.backend = backend
        self.backend_api = backend_api
        self.control_is_enable = False
        self.disconnection_counter = 0

        # Disable below warning-level logs
        log.getLogger("apscheduler.executors.default").setLevel(log.ERROR)
        log.getLogger("apscheduler.scheduler").setLevel(log.ERROR)

    def disable(self):
        """
        Disable running of all jobs, mainly to unplug COM.
        """
        self.periodic_job.pause()
        self.backend_api.app.comm_bar.status_lbl.config(text="Disconnected",background="#ff4c4c")

    def enable(self):
        """
        Enable module.
        """
        self.periodic_job.resume()

        # Local variables
        self.version_req_enable = True

    def periodic_vfp(self):
        """
        Run echo request and get uC status.
        Afterwards, a listener calls all the other jobs
        waiting in the queue.
        """
        # Run echo and get status
        log.info("Sched: periodic_vfp() = " + "Getting instrument status.")
        try:
            self.backend.comm_echo_req()
        except:
            log.warning("Sched: periodic_vfp() = " + "Instrument disconnected.")
            self.backend_api.be_set_connection_state(False, True)
            self.disconnection_counter += 1
        else:
            log.info("Sched: periodic_vfp() = " + "Instrument connected.")
            self.backend_api.be_set_connection_state(True)
            self._version_request()
            self.disconnection_counter = 0
        finally:
            if self.disconnection_counter >= 3:
                self.backend_api.be_set_connection_state(False, False)


    def add_job(self, func, args: list=None, next: bool=False):
        """
        Add job paused, waiting for next periodic_vfp
        to run only one time.

        Parameters:
            func: function to run
            args: arguments to pass to function
            next: whether the job should be run now or after next periodic
        """
        if self.backend_api.connected_state:

            # Generate random ID
            job_id = str(uuid.uuid4())

            # Add job paused
            self.sched.add_job(func=func, trigger="date", args=args, coalesce=True, next_run_time=None, id=job_id, max_instances=1)

            if next:
                self.queue_next.append(job_id)
            else:
                self.queue.append(job_id)

    def _version_request(self):
        if not self.version_req_enable:
            return

        fw_version, fw_date = self.backend.comm_version_req()

        # Update widget
        self.backend_api.app.side_panel.wg["fw"]["lbl_uc_r"].config(text=fw_version)
        self.backend_api.app.side_panel.wg["fw"]["lbl_last_build_r"].config(text=fw_date)

        # Disable version request
        self.version_req_enable = False

        # FIXME check bug fw_date doesnt display time

    def _queue_handler(self, event):
        """
        Listents to events of job executed or error.

        There are temporal jobs, added by main calling different functions
        and only one permanent, periodic_vfp. This function checks if there are
        any queued (temporal) jobs, pauses periodic_vfp if it founds any, runs them
        one after the other and then resumes periodic_vfp. If it doesn"t find any,
        it'll just skip.

        It also handles jobs queued for next run.
        """
        if event.code == EVENT_JOB_ERROR:
            log.error("Scheduler: _queue_handler() = " + "Scheduled job id {} failed".format(event.job_id))
        else:

            # If there are jobs queued, pause periodic_job and run them all
            if self.queue:
                if event.job_id == "periodic":
                    self.periodic_job.pause()

                # run and remove queued job from queue
                self.sched.modify_job(self.queue[0], next_run_time=datetime.now())
                self.queue.pop(0)

            # If there are no more jobs queued, check queue for next and resume periodic
            else:
                if self.queue_next:
                    for (i, job_id) in enumerate(self.queue_next):
                        self.queue.append(job_id)
                        self.queue_next.pop(i)
                if event.job_id != "periodic":
                    self.periodic_job.resume()
