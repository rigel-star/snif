import { useState } from "react";

export interface FilterBarProps {
    onApply?: (filters: string[]) => void;
}

export default function FilterBar(props: FilterBarProps) {
    const [filters, setFilters] = useState<string>("");

    const processFilters = () => {
        let processed = filters.split(",");
        if (processed.length === 1 && processed[0] === "") {
            return [];
        }
        return processed;
    };

	return (
		<div className="flex items-center gap-2 p-2">
			<span className="text-sm font-medium text-gray-700">
				Filter:
			</span>
			<input 
				type="text" 
				placeholder="Type to filter..."
				className="flex-1 px-3 py-1 border rounded-md focus:outline-none pl-2"
                onChange={event => {
                    setFilters(event.target.value);
                }}
			/>
            <button 
                className="bg-blue-500 rounded-sm px-3 py-1 text-white hover:bg-blue-600 transition-colors disabled:opacity-50 cursor-pointer"
                onClick={() => {
                    if (props.onApply) {
                        props.onApply(processFilters());
                    }
                }}
            >
                Apply
            </button>
		</div>
	);
};
