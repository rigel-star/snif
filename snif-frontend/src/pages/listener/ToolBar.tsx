import { useState } from "react";
import "./ToolBar.css";

export interface ToolBarProps {
    onStart?: () => void;
    onStop?: () => void;
}

export default function ToolBar(props: ToolBarProps) {
    const [hasStarted, setHasStarted] = useState<boolean>(false);

    return (
        <div id="toolbar">
            <button 
                className="snif-btn" 
                id="cap-start-btn"
                onClick={() => {
                    setHasStarted(true);
                    if (props.onStart) {
                        props.onStart();
                    }
                }}
                disabled={hasStarted}
            >
                Start
            </button>
            <button 
                className="snif-btn" 
                id="cap-stop-btn"
                disabled={!hasStarted}
                onClick={() => {
                    setHasStarted(false)
                    if (props.onStop) {
                        props.onStop();
                    }
                }}
            >
                Stop
            </button>
        </div>
    );
}