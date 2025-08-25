import { useState } from "react";
import FilterBar, { type FilterBarProps } from "./FilterBar";

export type ToolBarProps = {
    onStart?: () => void;
    onStop?: () => void;
} 
& FilterBarProps
& React.HTMLAttributes<HTMLDivElement>;

export default function ToolBar(props: ToolBarProps) {
    const [hasStarted, setHasStarted] = useState<boolean>(false);

    return (
        <div id="toolbar" className={`${props.className}`}>
            <div className="flex gap-4 flex-row">
                <button 
                    className="bg-blue-500 text-white border-2 border-blue-500 rounded-sm px-4 py-2 h-10 w-20 hover:bg-blue-600 transition-colors disabled:opacity-50"
                    id="cap-start-btn"
                    onClick={() => {
                        setHasStarted(true);
                        if (props.onStart) props.onStart();
                    }}
                    disabled={hasStarted}
                >
                    Start
                </button>
                <button 
                    className="bg-blue-500 text-white border-2 border-blue-500 rounded-sm px-4 py-2 h-10 w-20 hover:bg-blue-600 transition-colors disabled:opacity-50"
                    id="cap-stop-btn"
                    onClick={() => {
                        setHasStarted(false);
                        if (props.onStop) props.onStop();
                    }}
                    disabled={!hasStarted}
                >
                    Stop
                </button>
            </div>
            <FilterBar
                onApply={props.onApply}
            />
        </div>
    );
}